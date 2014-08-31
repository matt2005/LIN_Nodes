/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <stdint.h>
#include <unistd.h>
#include <err.h>
#include <getopt.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

#include <lin_defs.h>

#include "link.h"
#include "log.h"
#include "param.h"
#include "firmware.h"
#include "node.h"

void usage();

void
status(int argc, char *argv[])
{
    Link::print_status();
}

void
history(int argc, char *argv[])
{
    Log::acquire();
    Log::print();
}

void
trace(int argc, char *argv[])
{
    Log::trace();
}

void
scan(int argc, char *argv[])
{
    unsigned node = Node::kNoNode;
    int ch;

    while ((ch = getopt(argc, argv, "n:")) != -1) {
        switch (ch) {
        case 'n':
            node = strtoul(optarg, nullptr, 0);
            break;

        default:
            warnx("ERROR: unrecognised option '-%c'", ch);
            usage();
        }
    }

    argc -= optind;
    argv += optind;

    Node::scan(node);

    for (auto n : Node::nodes()) {
        auto pset = n->params();
        char *str = pset.identity();

        printf("%s", str);
        free(str);
    }
    Log::print();
}

void
dump_params(int argc, char *argv[])
{
    unsigned node = Node::kNoNode;
    int ch;

    while ((ch = getopt(argc, argv, "n:")) != -1) {
        switch (ch) {
        case 'n':
            node = strtoul(optarg, nullptr, 0);
            break;

        default:
            warnx("ERROR: unrecognised option '-%c'", ch);
            usage();
        }
    }

    argc -= optind;
    argv += optind;

    Node::scan(node);

    if (Node::exists(Bootloader::kNodeAddress) && (node == Node::kNoNode)) {
        errx(1, "ERROR: cannot save all parameters, there is a node that needs recovery");
    }

    const char *pfile = (argc > 1) ? argv[1] : "/dev/stdout";
    ParamDB pdb;

    for (auto n : Node::nodes()) {
        auto pset = n->params();
        pset.sync();
        pdb.store(pset);
    }

    pdb.write(pfile);
}

void
load_params(int argc, char *argv[])
{
    unsigned node = Node::kNoNode;
    int ch;

    while ((ch = getopt(argc, argv, "n:")) != -1) {
        switch (ch) {
        case 'n':
            node = strtoul(optarg, nullptr, 0);
            break;

        default:
            warnx("ERROR: unrecognised option '-%c'", ch);
            usage();
        }
    }

    argc -= optind;
    argv += optind;

    Node::scan(node);

    if (Node::exists(Bootloader::kNodeAddress) && (node == Node::kNoNode)) {
        errx(1, "ERROR: cannot load all parameters, there is a node that needs recovery");
    }

    const char *pfile = (argc > 1) ? argv[1] : "/dev/stdin";
    ParamDB pdb;

    try {
        pdb.read(pfile);

    } catch (std::runtime_error &e) {
        errx(1, "ERROR: reading %s: %s", pfile, e.what());
    }

    for (auto dbnode : pdb.nodes()) {
        unsigned nodeAddress = dbnode.second.get("node").toInt();
        unsigned nodeFunction = dbnode.second.get("function").toInt();

        auto node = Node::matching(nodeAddress, nodeFunction);

        if (node == nullptr) {
            if (Node::exists(nodeAddress)) {
                warnx("WARNING: node at %u does not match function %u.", nodeAddress, nodeFunction);

            } else {
                warnx("WARNING: node at %u is not responding.", nodeAddress);
            }

            continue;
        }

        auto pset = node->params();
        auto dbparams = dbnode.second.get("parameters");

        for (auto dbparam : dbparams) {
            try {
                pset.set(dbparam.second);

            } catch (std::runtime_error &e) {
                warnx("WARNING: node %u does not support parameter %s.",
                      nodeAddress, dbparam.second.get("name").toString().c_str());
            }
        }
    }
}

void
update(int argc, char *argv[])
{
    unsigned node = Node::kNoNode;
    int ch;

    for (unsigned i = 0; i < argc; i++) {
        warnx("%u: %s", i, argv[i]);
    }

    while ((ch = getopt(argc, argv, "n:")) != -1) {
        switch (ch) {
        case 'n':
            node = strtoul(optarg, nullptr, 0);
            warnx("node %u", node);
            break;

        default:
            warnx("ERROR: unrecognised option '-%c'", ch);
            usage();
        }
    }

    argc -= optind;
    argv += optind;

    Node::scan(node);

    for (unsigned arg = 0; arg < argc; arg++) {
        try {
            warnx("fw: %u %s", arg, argv[arg]);
            new Firmware(argv[arg]);

        } catch (std::runtime_error &e) {
            errx(1, "ERROR: loading firmware from %s: %s", argv[arg], e.what());
        }
    }

    for (auto n : Node::nodes()) {
        try {
            n->update();

        } catch (Exception &e) {
            warnx("WARNING: failed updating node@%u: %s", n->address(), e.what());
        }
    }
}

struct {
    const char  *cmd;
    const char *help;
    void (*func)(int argc, char *argv[]);
} commands[] = {
    {
        "status",
        "lintool status\n"
        "    Print link status.\n",
        status
    },
    {
        "history",
        "lintool history\n"
        "    Dump recent history from link.\n",
        history
    },
    {
        "trace",
        "lintool trace\n"
        "    Trace LIN traffic (^C to exit).\n",
        trace
    },
    {
        "scan",
        "lintool [-l] scan [-n <node>]\n"
        "    Scan for a specific node or list all nodes.\n",
        scan
    },
    {
        "dump_params",
        "lintool [-l] dump_params [-n <node>] [<file>]\n"
        "    Dump parameters to file (or stdout if <file> not specified).\n",
        dump_params
    },
    {
        "load_params",
        "lintool [-l] load_params [-n <node>] [<file>]\n"
        "    Load parameters fril file (or stdin if <file> not specified).\n",
        load_params
    },
    {
        "update",
        "lintool [-l] update [-n <node>] <file> [<file>]\n"
        "    Update node firmware for one or more nodes from one or more firmware files.\n"
        "    Only nodes for which firmware is loaded can be updated. If -n is not specified,\n"
        "    all nodes will be updated.\n"
        "    To update a node in recovery mode that has lost its type, pass -n 32 and supply\n"
        "    only one firmware file. The node will take the identity specified in the file.\n",
        update
    }
};

void
usage()
{
    warnx("Usage:");
    fprintf(stderr, "Common options:\n");
    fprintf(stderr, "        -l  enable logging\n");

    for (auto cmd : commands) {
        fprintf(stderr, "%s", cmd.help);
    }

    exit(1);
}

int
main(int argc, char *argv[])
{
    int ch;

    while ((ch = getopt(argc, argv, "l")) != -1) {
        switch (ch) {
        case 'l':
            Log::enable = true;
            break;

        default:
            warnx("ERROR: unrecognised option '-%c'", ch);
            usage();
        }
    }

    argc -= optind;
    argv += optind;
    optreset = 1;
    optind = 1;

    try {
        Link::connect();

    } catch (Exception &e) {
        errx(1, "connection failed: %s", e.what());
    }

    for (auto cmd : commands) {
        if (!strcmp(cmd.cmd, argv[0])) {
            try {
                cmd.func(argc, argv);

            } catch (Exception &e) {
                errx(1, "%s failed: %s", cmd.cmd, e.what());
                Log::print();
            }

            exit(0);
        }
    }

    warnx("ERROR: unrecognised command '%s'", argv[0]);
    usage();
}

