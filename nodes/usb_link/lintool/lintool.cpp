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

#include <stdexcept>

#include "link.h"
#include "log.h"

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
print_parameters()
{
    Link::enable_master(true);
    Link::set_node(32);

    uint16_t value = 0xffff;

    try {
        value = Link::read_data(Generic::kParamProtocolVersion);
        warnx("%d: got %d", 1, value);

    } catch (std::runtime_error &re) {
        warnx("read failed: %s", re.what());
        Log::log(false);
    }

    Link::enable_master(false);

}

void
bl_dump_memory()
{
    Link::enable_master(true);
    Link::set_node(32);

    for (unsigned address = 0; address < 16384; address++) {
        Link::write_data(Bootloader::kParamDebugPointer, address);
        unsigned readback = Link::read_data(Bootloader::kParamDebugPointer);
        if (readback != address) {
            Log::log(false);
            errx(1, "address pointer readback error, got %u expected %u", readback, address);
        }
        uint8_t value = Link::read_data(Bootloader::kParamMemory);
        warnx("%d: %02x", address, value);
    }
}

int
main(int argc, const char *argv[])
{
    Link::connect();

    if (argc == 2) {

        if (!strcmp(argv[1], "-status")) {
            print_status();
            exit(0);
        }

        if (!strcmp(argv[1], "-history")) {
            Log::log(false);
            exit(0);
        }

        Log::clear();

        if (!strcmp(argv[1], "-trace")) {
            Log::log(true);
            exit(0);
        }

        if (!strcmp(argv[1], "-master")) {
            Link::enable_master(true);
            exit(0);
        }

        if (!strcmp(argv[1], "-dump_params")) {
            print_parameters();
            Log::log(false);
            exit(0);
        }

        if (!strcmp(argv[1], "-bl_dump_memory")) {
            bl_dump_memory();
            exit(0);
        }
    }

    errx(1, "must supply one of -status, -trace, -master, -dump_params -bl_dump_memory");
}

