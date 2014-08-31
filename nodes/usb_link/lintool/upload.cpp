/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <unistd.h>
#include <err.h>

#include "upload.h"
#include "link.h"
#include "log.h"

#include <lin_defs.h>

namespace Upload
{

unsigned pagesize;

uint8_t
lo8(uint16_t val)
{
    return val & 0xff;
}

uint8_t
hi8(uint16_t val)
{
    return val >> 8;
}

uint16_t
crc_ccitt_update(uint16_t crc, uint8_t data)
{
    data ^= lo8(crc);
    data ^= data << 4;
    return ((((uint16_t)data << 8) | hi8(crc)) ^ (uint8_t)(data >> 4) ^ ((uint16_t)data << 3));
}

unsigned
page_crc(uint8_t *bytes)
{
    uint16_t crc = 0xffff;
    unsigned count = pagesize;

    while (count--) {
        crc = crc_ccitt_update(crc, *bytes++);
    }

    return crc;
}

unsigned
status()
{
    // this should block until the link can get a value from the bootloader
    return Link::read_param(Bootloader::kParamStatus);
}

void
check_ready()
{
    unsigned s = status();

    if ((s == bl_status::kWaitingForProgrammer) ||
        (s == bl_status::kReadyForPage)) {
        return;
    }

    if (s == bl_status::kPageAddressError) {
        RAISE(ExBadFirmware, "bootloader not ready: address error");
    }

    if (s == bl_status::kPageCRCError) {
        RAISE(ExCRCError, "bootloader not ready: CRC error");
    }

    if (s == bl_status::kPageInProgress) {
        RAISE(ExProtocol, "bootloader not ready: page still/already in progress");
    }

    RAISE(ExProtocol, "bootloader not ready, unexpected status: " << s);
}

void
reset_bootloader()
{
    Link::write_param(Generic::kParamBootloaderMode, 1);
}

void
enter_bootloader()
{
    try {
        if (Link::read_param(Generic::kParamBootloaderMode) > 0) {
            return;
        }

        Link::write_param(Generic::kParamBootloaderMode, bootloader_magic::kEnterBootloader);
        usleep(50000);

        if (Link::read_param(Generic::kParamBootloaderMode) == 0) {
            RAISE(ExProtocol, "app failed to enter bootloader");
        }

    } catch (ExProtocol &e) {
        throw;

    } catch (Exception &e) {
        RAISE(ExLink, "trying to enter bootloader mode: " << e.what());
    }
}

void
leave_bootloader()
{
    try {

        if (Link::read_param(Generic::kParamBootloaderMode) == 0) {
            return;
        }

        Link::write_param(Generic::kParamBootloaderMode, 0);
        usleep(50000);

        if (Link::read_param(Generic::kParamBootloaderMode) != 0) {
            RAISE(ExProtocol, "bootloader reboot failed");
        }

    } catch (ExProtocol &e) {
        throw;

    } catch (Exception &e) {
        RAISE(ExLink, "trying to leave bootloader mode: " << e.what());
    }
}

void
set_address(unsigned address)
{
    Link::write_param(Bootloader::kParamPageAddress, address);

    if (status() == bl_status::kPageAddressError) {
        RAISE(ExBadFirmware, "bootloader rejected firmware due to address error");
    }
}

void
program_page(unsigned address, uint8_t *bytes)
{
    unsigned crc = page_crc(bytes);

    check_ready();

    for (unsigned tries = 0; tries < 3; tries++) {

        uint16_t running_crc = 0xffff;

        // set the page address
        set_address(address);

        // upload data to the page buffer
        for (unsigned offset = 0; offset < pagesize; offset += 4) {
            Link::bulk_data(bytes + offset);
            running_crc = crc_ccitt_update(running_crc, bytes[offset + 0]);
            running_crc = crc_ccitt_update(running_crc, bytes[offset + 1]);
            running_crc = crc_ccitt_update(running_crc, bytes[offset + 2]);
            running_crc = crc_ccitt_update(running_crc, bytes[offset + 3]);
            unsigned bl_crc = Link::read_param(Bootloader::kParamPageCRC);
            if (bl_crc != running_crc) {
                Log::print();
                errx(1, "%u: crc bl %04x ours %04x", offset, bl_crc, running_crc);
            }
        }

        if (running_crc != crc) {
            warnx("crc weird, running %04x, static %04x", running_crc, crc);
        }

        // send our CRC to the bootloader; it will either program the
        // page or it will flag a CRC error
        Link::write_param(Bootloader::kParamPageCRC, crc);

        // wait for the page to be programmed
        try {
            check_ready();

        } catch (ExCRCError &e) {
            // we are prepared to retry here...
            warnx("CRC error (%u) we have %x bootloader wants %x",
                  tries, crc, Link::read_param(Bootloader::kParamPageCRC));
            reset_bootloader();
            continue;
        }

        return;
    }

    RAISE(ExLink, "too many upload CRC errors");
}

void
upload(Firmware *fw)
{
    pagesize = Link::read_param(Generic::kParamFirmwarePageSize);

    enter_bootloader();
    reset_bootloader();

    if (status() != bl_status::kWaitingForProgrammer) {
        RAISE(ExProtocol, "bootloader in unexpected state");
    }

    // loop writing pages
    unsigned address = pagesize;
    uint8_t bytes[pagesize];

    while (address < fw->max_address()) {

        if (fw->get_bytes(address, pagesize, &bytes[0])) {
            warnx("program: 0x%04x", address);
            program_page(address, &bytes[0]);
        }

        address += pagesize;
    }

//    if (fw->get_bytes(0, pagesize, &bytes[0])) {
//        program_page(0, &bytes[0]);
//    }

    // XXX verify upload?

    leave_bootloader();

}

} // namespace Upload
