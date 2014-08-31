/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <unistd.h>

#include "upload.h"
#include "link.h"

#include <lin_defs.h>

namespace Upload
{

unsigned pagesize;

unsigned
status()
{
    return Link::read_data(Bootloader::kParamStatus);
}

void
wait_until_ready()
{
    for (unsigned tries = 0; tries < 50; tries++) {
        unsigned s = status();

        if ((s == bl_status::kWaitingForProgrammer) ||
            (s == bl_status::kReadyForPage)) {
            return;
        }
        usleep(10000);
    }
    throw (std::runtime_error("timed out waiting for bootloader to be ready"));
}

void
enter_bootloader()
{
    if (Link::read_data(Generic::kParamBootloaderMode) > 0) {
        return;
    }
    Link::write_data(Generic::kParamBootloaderMode, bootloader_magic::kEnterBootloader);
    usleep(50000);
    if (Link::read_data(Generic::kParamBootloaderMode) == 0) {
        throw (std::runtime_error("failed to enter bootloader mode"));
    }
}

void
leave_bootloader()
{
    if (Link::read_data(Generic::kParamBootloaderMode) == 0) {
        return;
    }
    Link::write_data(Generic::kParamBootloaderMode, 0);
    usleep(50000);
    if (Link::read_data(Generic::kParamBootloaderMode) != 0) {
        throw (std::runtime_error("failed to leave bootloader mode"));
    }
}

void
reset_bootloader()
{
    Link::write_data(Generic::kParamBootloaderMode, 1);
}

void
set_address(unsigned address)
{
    Link::write_data(Bootloader::kParamPageAddress, address);
    if (status() == bl_status::kPageAddressError) {
        throw (std::runtime_error("bootloader rejected firmware due to address error"));
    }
}

unsigned
crc_xmodem(uint8_t *bytes, unsigned count, unsigned initial = 0)
{
    unsigned crc = initial;

    while (count) {
        crc ^= *bytes++;
        for (unsigned i = 0; i < 8; i++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            } else {
                crc <<= 1;
            }
            crc &= 0xffff;
        }
    }
    return crc;
}

void
program_page(unsigned address, uint8_t *bytes)
{
    unsigned crc = crc_xmodem(bytes, pagesize);

    for (unsigned tries = 0; tries < 3; tries++) {
        wait_until_ready();
        set_address(address);

        for (unsigned offset = 0; offset < pagesize; offset += 4) {
            Link::bulk_data(bytes + offset);
        }

        try {
            Link::write_data(Bootloader::kParamPageCRC, crc);
        } catch (std::runtime_error e) {
            continue;
        }
        return;
    }
    throw (std::runtime_error("too many upload CRC errors, aborting"));
}

void
upload(Firmware *fw)
{
    pagesize = Link::read_data(Generic::kParamFirmwarePageSize);

    enter_bootloader();
    reset_bootloader();

    if (status() != bl_status::kWaitingForProgrammer) {
        throw (std::runtime_error("bootloader in unexpected state"));
    }

    // loop writing pages
    unsigned address = pagesize;
    uint8_t bytes[pagesize];
    while (address < fw->max_address()) {

        if (fw->get_bytes(address, pagesize, &bytes[0])) {
            program_page(address, &bytes[0]);
        }
        address += pagesize;
    }
    if (fw->get_bytes(0, pagesize, &bytes[0])) {
        program_page(0, &bytes[0]);
    }

    // XXX verify upload?

    leave_bootloader();

}

} // namespace Upload
