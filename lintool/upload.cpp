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
page_crc(uint8_t *bytes, uint16_t address)
{
    uint16_t crc = 0xffff;
    unsigned count = pagesize;

    crc = crc_ccitt_update(crc, address & 0xff);
    crc = crc_ccitt_update(crc, address >> 8);

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
    auto s = status();

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
    Link::write_param(Generic::kParamOperationMode, 1);
}

void
set_bootloader(bool wantBootloader)
{
    auto desired = (wantBootloader ? operation_magic::kBootloader : operation_magic::kProgram);
    auto command = (wantBootloader ? operation_magic::kEnterBootloader : operation_magic::kProgram);

    try {

        if (Link::read_param(Generic::kParamOperationMode) == desired) {
            // already in the desired state, nothing to do here
            return;
        }

        Link::write_param(Generic::kParamOperationMode, command);

        // We might have just tried to reboot the master node, and it might not have
        // taken, so go through the master setup process again just in case we need to
        // kick it off the bus once more. This will let us report the error more accurately,
        // rather than giving a cryptic link error due to two masters trying to talk...
        //
        // Note that it's not necessary to re-enable master mode; it will happen automatically
        // when we try to read the bootloader mode parameter
        //
        // XXX should only do this dance when we are talking to the master node...
        //
        Link::enable_master(false);
        usleep(500000);             // 500ms seems to work OK

        // wait for the node to come back up in bootloader mode
        for (auto tries = 0; tries < 50; tries++) {

            try {
                auto state = Link::read_param(Generic::kParamOperationMode);

                if (state == desired) {
                    return;
                }

                usleep(20000);

            } catch (Link::ExLINError &e) {
                // transfer failed; node is probably still rebooting
                continue;
            }
        }

        RAISE(ExProtocol, "trying to " << (wantBootloader ? "enter" : "leave") << " bootloader mode");

    } catch (ExProtocol &e) {
        throw;

    } catch (Exception &e) {
        RAISE(ExLink, "trying to " << (wantBootloader ? "enter" : "leave") << " bootloader mode: " << e.what());
    }

}

void
enter_bootloader()
{
    set_bootloader(true);
}

void
leave_bootloader()
{
    set_bootloader(false);
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
    for (auto tries = 0; tries < 3; tries++) {
        auto value = Link::read_param(Bootloader::kParamMemory);
        auto set_address = Link::read_param(Bootloader::kParamDebugPointer);

        if (set_address == (address + 1)) {
            return value;
        }

        Link::write_param(Bootloader::kParamDebugPointer, address);
    }

    RAISE(ExBadAddress, "can't set bootloader readback address");
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
    unsigned crc = page_crc(bytes, address);

    check_ready();

    for (auto tries = 0; tries < 3; tries++) {

        warnx("program: 0x%04x%s", address, (tries > 0) ? " (retry)" : "");

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
            warnx("verify: 0x%04x", address);

            for (unsigned offset = 0; offset < pagesize; offset++) {
                // readback for reset vector will never compare, so ignore it
                if ((address + offset) <= 3) {
                    continue;
                }

                for (unsigned readback_tries = 0;; readback_tries++) {

                    if (read_bl_memory(address + offset) == bytes[offset]) {
                        break;
                    }

                    if (readback_tries > 3) {
                        RAISE(ExVerifyError, "address " << address + offset << " persistent readback miscompare");
                    }
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
    for (auto offset = 0U; offset < size; offset += 8) {
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
    for (auto offset = 0U; offset < size; offset += 4) {
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

    leave_bootloader();
}

} // namespace Upload
