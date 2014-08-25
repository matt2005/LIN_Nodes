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

#include "../../../common/lin_defs.h"


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
dump_params(unsigned node)
{
    Log::clear();

    auto set = new ParamSet(node);
    set->print();
    delete set;
}

void
print_parameters(unsigned node)
{
    Log::clear();
    Link::set_node(node);
    Link::enable_master(true);
    warnx("dumping node %u", node);

    for (unsigned address = 0; address < 0x10000; address++) {
        if (Generic::param_exists(address)) {
            printf("%s %u\n", Generic::param_name(address), Link::read_data(address));
            Log::acquire();
        }
    }

    if (Link::read_data(Generic::kParamBootloaderMode) != 0) {
        for (unsigned address = 0; address < 0x10000; address++) {
            if (Bootloader::param_exists(address)) {
                printf("%s %u\n", Bootloader::param_name(address), Link::read_data(address));
                Log::acquire();
            }
        }
    } else {
        switch (Link::read_data(Generic::kParamBoardFunction)) {
        case board_function::kMaster:
            for (unsigned address = 0; address < 0x10000; address++) {
                if (Master::param_exists(address)) {
                    printf("%s %u\n", Master::param_name(address), Link::read_data(address));
                    Log::acquire();
                }
            }
            break;
        case board_function::kPowerV1:
            for (unsigned address = 0; address < 0x10000; address++) {
                if (PowerV1::param_exists(address)) {
                    printf("%s %u\n", PowerV1::param_name(address), Link::read_data(address));
                    Log::acquire();
                }
            }
            break;
        case board_function::kPowerV3:
            for (unsigned address = 0; address < 0x10000; address++) {
                if (PowerV3::param_exists(address)) {
                    printf("%s %u\n", PowerV3::param_name(address), Link::read_data(address));
                    Log::acquire();
                }
            }
            break;
        }
    }
    Link::enable_master(false);
    Log::print();
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
    warnx("usage: lintool [-l][-h][-n <node>][-f <file>] <command>");
    warnx("    -l  enable logging");
    warnx("    -n  set node address for some commands");
    warnx("    -f  add a firmware file (may be specified more than once)");
    warnx("    -h  print this help");
    warnx("  <command>:");
    warnx("    status        Print link status");
    warnx("    history       Dump recent history from link");
    warnx("    trace         Trace LIN traffic (^C to exit)");
    warnx("    dump_params   Dump all parameters for <node>");
    exit(1);
}

int
main(int argc, char *argv[])
{
    int ch;
    unsigned node = Bootloader::kNodeAddress;

    while ((ch = getopt(argc, argv, "f:ln:h")) != -1) {
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

    if (!strcmp(argv[0], "history")) {
        Log::acquire();
        Log::print();
        exit(0);
    }

    if (!strcmp(argv[0], "trace")) {
        Log::trace();
        exit(0);
    }

    if (!strcmp(argv[0], "dump_params")) {
//        print_parameters(node);
        dump_params(node);
        exit(0);
    }

    if (!strcmp(argv[0], "bl_dump_memory")) {
        bl_dump_memory(node);
        exit(0);
    }

    warnx("unrecognised command '%s'", argv[0]);
    usage();
}

