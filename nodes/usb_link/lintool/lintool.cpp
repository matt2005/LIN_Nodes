/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <err.h>
#include <getopt.h>

#include <stdexcept>

#include "link.h"
#include "log.h"
#include "param.h"
#include "firmware.h"
#include "node.h"

#include "../../../common/lin_defs.h"

void
scan(unsigned node)
{
    Node::scan(node);

    for (auto n : Node::nodes()) {
        auto pset = n->params();
        char *str = pset->identity();

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

    FILE *fp;

    if (pfile == nullptr) {
        fp = stdout;
    } else {
        fp = fopen(pfile, "w");
        if (fp == nullptr) {
            err(1, "open %s", pfile);
        }
    }

    for (auto n : Node::nodes()) {
        auto pset = n->params();

        pset->print(fp);
    }
    fclose(fp);
}

void
print_status()
{
    warnx("free memory: %u", Link::get_status(RQ_STATUS_FREEMEM));
    uint8_t status = Link::get_status();

    if (status & RQ_STATUS_DATA_READY) {
        warnx("status: DATA_READY");
    }

    if (status & RQ_STATUS_DATA_ERROR) {
        warnx("status: DATA_ERROR");
    }

    if (status & RQ_STATUS_AWAKE) {
        warnx("status: AWAKE");
    }

    if (status & RQ_STATUS_WAITING) {
        warnx("status: WAITING");
    }

    if (status & RQ_STATUS_MASTER) {
        warnx("status: MASTER");
    }

    warnx("Errors:");
    warnx("  line:            %u", Link::get_status(RQ_STATUS_LINERR, 0));
    warnx("  checksum:        %u", Link::get_status(RQ_STATUS_LINERR, 1));
    warnx("  parity:          %u", Link::get_status(RQ_STATUS_LINERR, 2));
    warnx("  framing:         %u", Link::get_status(RQ_STATUS_LINERR, 3));
    warnx("  synchronisation: %u", Link::get_status(RQ_STATUS_LINERR, 4));
    warnx("  protocol:        %u", Link::get_status(RQ_STATUS_LINERR, 5));
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
    fprintf(stderr, "Common options:")
    fprintf(stderr, "        -l  enable logging");
    fprintf(stderr, "        -n  set specific node address");
    fprintf(stderr, "        -p  specify a parameter file");
    fprintf(stderr, "");
    fprintf(stderr, "lintool [-h]");
    fprintf(stderr, "    Print this help.");
    fprintf(stderr, "");
    fprintf(stderr, "lintool trace");
    fprintf(stderr, "    Dump recent history from link.");
    fprintf(stderr, "");
    fprintf(stderr, "lintool trace");
    fprintf(stderr, "    Trace LIN traffic (^C to exit).");
    fprintf(stderr, "");
    fprintf(stderr, "lintool [-l] status");
    fprintf(stderr, "    Print link status.");
    fprintf(stderr, "");
    fprintf(stderr, "lintool [-l][-n <node>] <scan>");
    fprintf(stderr, "    Scan for a specific node or list all nodes.");
    fprintf(stderr, "");
    fprintf(stderr, "lintool [-l][-n <node>][-p <file>] dump_params");
    fprintf(stderr, "    Dump parameters to file (or stdout if -p not specified).");
    fprintf(stderr, "");
    fprintf(stderr, "lintool [-l][-n <node>][-p <file>] load_params");
    fprintf(stderr, "    Load parameters fril file (or stdin if -p not specified).");
    fprintf(stderr, "");
    fprintf(stderr, "lintool [-l][-n <node>] -f <file> [-f <file> ...] update");
    fprintf(stderr, "    Update node firmware for one or more nodes. Only nodes for which firmware")
    fprintf(stderr, "    is loaded can be updated. If -n is not specified, all nodes will be updated.")
    fprintf(stderr, "        -f  add a firmware file (may be specified more than once)");
    fprintf(stderr, "");
    fprintf(stderr, "lintool [-l] -f <file> recover");
    fprintf(stderr, "    Recover a node in bootloader mode. If the node advertises a type, it must")
    fprintf(stderr, "    match a type in the specified firmware file.")
    fprintf(stderr, "        -f  specify a firmware file");
    fprintf(stderr, "");
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
            new Firmware(optarg);
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
            warnx("unrecognised argument '-%c'", ch);
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
        print_status();
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

