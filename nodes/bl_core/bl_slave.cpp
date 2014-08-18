/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <avr/boot.h>
#include <util/crc16.h>

#include <board.h>

#include "bl_slave.h"

uint16_t        BLSlave::_send_index = kNoSendResponse;
uint16_t        BLSlave::_page_address = 0;
uint8_t         BLSlave::_page_offset = 0;
uint16_t        BLSlave::_running_crc = 0;
uint16_t        BLSlave::_page_status = bl_status::kWaitingForProgrammer;
uint16_t        BLSlave::_program_end = 0;
uint16_t        BLSlave::_reset_vector = 0;
uint16_t        BLSlave::_page_buffer[SPM_PAGESIZE / 2];

void
BLSlave::st_header_received()
{
    switch (current_FrameID()) {
    case kFrameIDMasterRequest:
        st_expect_response();
        break;

    case kFrameIDSlaveResponse:
        if (_send_index != kNoSendResponse) {
            send_response();
        }

        break;

    default:
        break;
    }
}

void
BLSlave::st_response_received(Response &resp)
{
    // reset some state
    _send_index = kNoSendResponse;

    switch (current_FrameID()) {
    case kFrameIDMasterRequest:

        // device being programmed is always node 32
        if (resp.MasterRequest.nad != Bootloader::kNodeAddress) {
            break;
        }

        switch (resp.MasterRequest.sid) {
        case service_id::kReadDataByID:
            _send_index = resp.DataByID.index;
            break;

        case service_id::kWriteDataByID:
            switch (resp.DataByID.index) {
            case Bootloader::kParamPageAddress:
                set_page_address(resp.DataByID.value);
                break;

            case Bootloader::kParamPageCRC:

                // pad the buffer out with 0xff
                while (add_page_byte(0xff)) {
                }

                // if the CRC matches, program the page
                if (resp.DataByID.value == _running_crc) {
                    program_page();
                    _page_status = bl_status::kReadyForPage;

                } else {
                    _page_status = bl_status::kPageCRCError;
                }
            }

            break;

        case service_id::kDataDump:

            // add bytes to the page buffer
            if (_page_status == bl_status::kPageInProgress) {
                uint8_t count = resp.MasterRequest.length;
                uint8_t field = 3;

                while (count--) {
                    add_page_byte(resp._bytes[field++]);
                }
            }

            break;

        default:
            break;
        }

        break;
    }
}

void
BLSlave::send_response()
{
    uint16_t value = 0;

    switch (_send_index) {
    case Generic::kParamProtocolVersion:
        // XXX should come from the defs file
        value = 1;
        break;

    case Generic::kParamBootloaderMode:
        value = 1;
        break;

    case Generic::kParamFirmwarePageSize:
        value = SPM_PAGESIZE;
        break;

    case Bootloader::kParamPageAddress:
        value = _page_address;
        break;

    case Bootloader::kParamPageCRC:
        value = _running_crc;
        break;

    case Bootloader::kParamStatus:
        value = _page_status;
        break;

    default:
        break;
    }

    _send_index = kNoSendResponse;

    Response resp;
    resp.DataByID.nad = 32;
    resp.DataByID.pci = pci::kSingleFrame;
    resp.DataByID.length = 5;
    resp.DataByID.sid = (service_id::kReadDataByID | service_id::kResponseOffset);
    resp.DataByID.index = _send_index;
    resp.DataByID.value = value;

    st_send_response(resp);
}

bool
BLSlave::is_program_valid()
{
    return get_program_crc(pgm_read_word(kInfoPage + 4)) == pgm_read_word(kInfoPage + 0);
}

void
BLSlave::run_program()
{
    typedef void (*Func)();

    Func entrypoint = (Func)pgm_read_word(kInfoPage + 2);

    entrypoint();
    Board::panic(Board::kPanicCodeAssert);
}

void
BLSlave::set_page_address(uint16_t address)
{
    _page_address = address;

    // kill the current program info, since we are starting a new program
    if (_page_status == bl_status::kWaitingForProgrammer) {
        update_program_info();
    }

    // set the new page address and prepare for the page
    if ((_page_address < BL_ADDR) && !(_page_address % SPM_PAGESIZE)) {
        _page_offset = 0;
        _running_crc = 0;
        _page_status = bl_status::kPageInProgress;

    } else {
        _page_status = bl_status::kPageAddressError;
    }
}

bool
BLSlave::add_page_byte(uint8_t byte)
{
    if (_page_offset < SPM_PAGESIZE) {
        _running_crc = _crc16_update(_running_crc, byte);
        uint8_t *p = (uint8_t *)&_page_buffer[0];
        p[_page_offset++] = byte;
        return true;
    }

    return false;
}

void
BLSlave::program_page()
{
    // patch our reset vector into the buffer
    if (_page_address == 0) {
        if (_page_buffer[0] == 0x940c) {
            // copy jmp argument
            _reset_vector = _page_buffer[1];

        } else {
            // extract rjmp destination
            _reset_vector = ((_page_buffer[0] & ~0xc000) + 1) * 2;
            // convert to jmp
            _page_buffer[0] = 0x940c;
        }

        // patch in bootloader reset vector
        _page_buffer[1] = BL_ADDR;
    }

    boot_page_erase(_page_address);
    boot_spm_busy_wait();

    for (uint8_t i = 0; i < (SPM_PAGESIZE / 2); i++) {
        boot_page_fill(_page_address + (i * 2), _page_buffer[i]);
    }

    boot_page_write(_page_address);
    boot_spm_busy_wait();

    // track the end of the programmed area
    _page_address += SPM_PAGESIZE;

    if (_page_address > _program_end) {
        _program_end = _page_address;
    }
}

void
BLSlave::update_program_info()
{

    boot_page_erase(kInfoPage);
    boot_spm_busy_wait();

    boot_page_fill(kInfoPage + 0, get_program_crc(_program_end));
    boot_page_fill(kInfoPage + 2, _reset_vector);
    boot_page_fill(kInfoPage + 4, _program_end);

    for (uint8_t offset = 6; offset < SPM_PAGESIZE; offset += 2) {
        boot_page_fill(kInfoPage + offset, 0xffff);
    }

    boot_page_write(kInfoPage);
    boot_spm_busy_wait();
}

uint16_t
BLSlave::get_program_crc(uint16_t length)
{
    uint16_t crc = 0;

    for (uint16_t ptr = 0; ptr < length; ptr++) {
        crc = _crc16_update(crc, pgm_read_byte(ptr));
    }

    return crc;
}
