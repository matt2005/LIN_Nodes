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
    try {
        return Link::read_param(Bootloader::kParamStatus);

    } catch (Exception &e) {
        RAISE(ExLink, "error reading bootloader status: " << e.what());
    }
}

void
check_ready()
{
    unsigned s = status();

    if ((s == bl_status::kWaitingForProgrammer) ||
        (s == bl_status::kReadyForPage)) {
        return;
    }

    // XXX we can remove this, check at end of page should catch it
    if (s == bl_status::kPageAddressError) {
        RAISE(ExBadAddress, "bootloader not ready: address error");
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

        warnx("rebooting node into bootloader");
        Link::write_param(Generic::kParamBootloaderMode, bootloader_magic::kEnterBootloader);
        usleep(50000);

        if (Link::read_param(Generic::kParamBootloaderMode) == 0) {
            RAISE(ExProtocol, "node failed to enter bootloader");
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
        usleep(500000);     // XXX should poll more aggressively...

        if (Link::read_param(Generic::kParamBootloaderMode) != 0) {
            RAISE(ExProtocol, "bootloader reboot failed: " 
                << Encoding::info(kEncoding_bl_reason, Link::read_param(Bootloader::kParamReason)));
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

    // XXX we can remove this, check at end of page should catch it
    if (status() == bl_status::kPageAddressError) {
        RAISE(ExBadAddress, "bootloader rejected firmware due to address error");
    }
}

unsigned
read_bl_memory(unsigned address)
{
    Link::write_param(Bootloader::kParamDebugPointer, address);
    return Link::read_param(Bootloader::kParamMemory);
}

unsigned
read_bl_eeprom(unsigned address)
{
    Link::write_param(Bootloader::kParamDebugPointer, address);
    return Link::read_param(Bootloader::kParamEEPROM);    
}

void
program_page(unsigned address, uint8_t *bytes, bool readback)
{
    unsigned crc = page_crc(bytes);

    check_ready();

    warnx("program: 0x%04x", address);

    for (unsigned tries = 0; tries < 3; tries++) {

        // set the page address
        set_address(address);

        // upload data to the page buffer
        for (unsigned offset = 0; offset < pagesize; offset += 4) {
            Link::bulk_data(bytes + offset);
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
        } catch (ExProtocol &e) {
            // write may have been dropped - page still thinks it wants more data
            warnx("page write failed (%u)", tries);
            continue;
        } catch (ExBadAddress &e) {
            // address write may have been corrupted
            warnx("page address failed (%u)", tries);
            continue;
        }

        if (readback) {
            for (unsigned offset = 0; offset < pagesize; offset++) {
                uint8_t is = read_bl_memory(address + offset);

                if (is != bytes[offset]) {
                    warnx("VERIFY: 0x%04x is 0x%02x should 0x%02x", address + offset, is, bytes[offset]);
                }
            }
        }

        return;
    }

    RAISE(ExLink, "too many upload CRC errors");
}

void
dump_page(unsigned address, unsigned size)
{
    for (unsigned offset = 0; offset < size; offset += 8) {
        warnx("%04x: %02x %02x %02x %02x %02x %02x %02x %02x",
              address + offset,
              read_bl_memory(address + offset + 0),
              read_bl_memory(address + offset + 1),
              read_bl_memory(address + offset + 2),
              read_bl_memory(address + offset + 3),
              read_bl_memory(address + offset + 4),
              read_bl_memory(address + offset + 5),
              read_bl_memory(address + offset + 6),
              read_bl_memory(address + offset + 7));
    }
}

void
dump_eeprom(unsigned address, unsigned size)
{
    for (unsigned offset = 0; offset < size; offset += 4) {
        warnx("%03x: %02x %02x %02x %02x",
              address + offset,
              read_bl_eeprom(address + offset + 0),
              read_bl_eeprom(address + offset + 1),
              read_bl_eeprom(address + offset + 2),
              read_bl_eeprom(address + offset + 3));
    }
}

void
upload(Firmware *fw, bool readback)
{
    pagesize = Link::read_param(Generic::kParamFirmwarePageSize);

    enter_bootloader();
    reset_bootloader();

    warnx("bl_reason: %s",
        Encoding::info(kEncoding_bl_reason, Link::read_param(Bootloader::kParamReason)));

    if (status() != bl_status::kWaitingForProgrammer) {
        RAISE(ExProtocol, "bootloader in unexpected state");
    }

    // loop writing pages
    unsigned address = pagesize;
    uint8_t bytes[pagesize];

    while (address < fw->max_address()) {

        if (fw->get_bytes(address, pagesize, &bytes[0])) {
            program_page(address, &bytes[0], readback);
        }

        address += pagesize;
    }

    if (fw->get_bytes(0, pagesize, &bytes[0])) {
        program_page(0, &bytes[0], readback);
    }

//    dump_page(0, pagesize);
    dump_page(0x4000 - pagesize, 16);
    dump_eeprom(508, 4);

    leave_bootloader();

}

} // namespace Upload
