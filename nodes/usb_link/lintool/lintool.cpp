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

void
scan(unsigned node)
{
    Node::scan(node);

    for (auto n : Node::nodes()) {
        auto pset = n->params();
        pset.sync();
        char *str = pset.identity();

        printf("%s", str);
        free(str);
    }
}

void
dump_params(unsigned node, const char *pfile)
{
    Node::scan(node);

    if (Node::exists(Bootloader::kNodeAddress) && (node == Node::kNoNode)) {
        errx(1, "ERROR: cannot save all parameters, there is a node that needs recovery");
    }

    if (pfile == nullptr) {
        pfile = "/dev/stdout";
    }

    ParamDB pdb;

    for (auto n : Node::nodes()) {
        auto pset = n->params();
        pset.sync();
        pdb.store(pset);
    }

    pdb.write(pfile);
}

void
load_params(unsigned node, const char *pfile)
{
    Node::scan(node);

    if (Node::exists(Bootloader::kNodeAddress) && (node == Node::kNoNode)) {
        errx(1, "ERROR: cannot load all parameters, there is a node that needs recovery");
    }

    if (pfile == nullptr) {
        pfile = "/dev/stdin";
    }

    ParamDB pdb;

    try {
        pdb.read(pfile);

    } catch (std::runtime_error e) {
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

            } catch (std::runtime_error e) {
                warnx("WARNING: node %u does not support parameter %s.",
                      nodeAddress, dbparam.second.get("name").toString().c_str());
            }
        }
    }
}

void
bl_dump_memory(unsigned node)
{
    Log::clear();
    Link::enable_master(true);
    Link::set_node(node);

    for (unsigned address = 0; address < 16384; address++) {
        Link::write_data(Bootloader::kParamDebugPointer, address);
        unsigned readback = Link::read_data(Bootloader::kParamDebugPointer);
        Log::acquire();

        if (readback != address) {
            Log::print();
            errx(1, "address pointer readback error, got %u expected %u", readback, address);
        }

        uint8_t value = Link::read_data(Bootloader::kParamMemory);
        warnx("%d: %02x", address, value);
    }

    Log::print();
}


void
usage()
{
    warnx("Usage:");
    fprintf(stderr, "Common options:\n");
    fprintf(stderr, "        -l  enable logging\n");
    fprintf(stderr, "        -n  set specific node address\n");
    fprintf(stderr, "        -p  specify a parameter file\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "lintool [-h]\n");
    fprintf(stderr, "    Print this help.\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "lintool trace\n");
    fprintf(stderr, "    Dump recent history from link.\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "lintool trace\n");
    fprintf(stderr, "    Trace LIN traffic (^C to exit).\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "lintool [-l] status\n");
    fprintf(stderr, "    Print link status.\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "lintool [-l][-n <node>] <scan>\n");
    fprintf(stderr, "    Scan for a specific node or list all nodes.\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "lintool [-l][-n <node>][-p <file>] dump_params\n");
    fprintf(stderr, "    Dump parameters to file (or stdout if -p not specified).\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "lintool [-l][-n <node>][-p <file>] load_params\n");
    fprintf(stderr, "    Load parameters fril file (or stdin if -p not specified).\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "lintool [-l][-n <node>] -f <file> [-f <file> ...] update\n");
    fprintf(stderr, "    Update node firmware for one or more nodes. Only nodes for which firmware\n");
    fprintf(stderr, "    is loaded can be updated. If -n is not specified, all nodes will be updated.\n");
    fprintf(stderr, "        -f  add a firmware file (may be specified more than once)\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "lintool [-l] -f <file> recover\n");
    fprintf(stderr, "    Recover a node in bootloader mode. If the node advertises a type, it must\n");
    fprintf(stderr, "    match a type in the specified firmware file.\n");
    fprintf(stderr, "        -f  specify a firmware file\n");
    fprintf(stderr, "\n");
    exit(1);
}

int
main(int argc, char *argv[])
{
    int ch;
    unsigned node = Node::kNoNode;
    const char *pfile = nullptr;

    while ((ch = getopt(argc, argv, "f:hln:p:")) != -1) {
        switch (ch) {
        case 'f':
            try {
                new Firmware(optarg);

            } catch (std::runtime_error &e) {
                errx(1, "ERROR: loading firmware from %s: %s", optarg, e.what());
            }

            break;

        case 'l':
            Log::enable = true;
            break;

        case 'n':
            node = strtoul(optarg, 0, 0);
            break;

        case 'p':
            pfile = strdup(optarg);
            break;

        default:
            warnx("ERROR: unrecognised argument '-%c'", ch);

        // FALLTHROUGH
        case 'h':
            usage();
        }
    }

    argc -= optind;
    argv += optind;

    if (argc < 1) {
        warnx("missing command");
        usage();
    }

    Link::connect();

    if (!strcmp(argv[0], "status")) {
        Link::print_status();
        exit(0);
    }

    if (!strcmp(argv[0], "scan")) {
        scan(node);
        exit(0);
    }

    if (!strcmp(argv[0], "dump_params")) {
        dump_params(node, pfile);
        exit(0);
    }

    if (!strcmp(argv[0], "load_params")) {
        load_params(node, pfile);
        exit(0);
    }

    if (!strcmp(argv[0], "history")) {
        Log::acquire();
        Log::print();
        exit(0);
    }

    if (!strcmp(argv[0], "trace")) {
        Log::trace();
        exit(0);
    }

    if (!strcmp(argv[0], "bl_dump_memory")) {
        bl_dump_memory(node);
        exit(0);
    }


    warnx("unrecognised command '%s'", argv[0]);
    usage();
}

