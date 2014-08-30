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

uint16_t        BLSlave::_sendIndex = kNoSendResponse;
uint16_t        BLSlave::_pageAddress = 0;
uint8_t         BLSlave::_pageOffset = 0;
uint16_t        BLSlave::_runningCrc = 0;
uint16_t        BLSlave::_pageStatus = bl_status::kWaitingForProgrammer;
uint16_t        BLSlave::_programEnd = 0;
uint16_t        BLSlave::_resetVector = 0;
uint16_t        BLSlave::_pageBuffer[SPM_PAGESIZE / 2];
uint16_t        BLSlave::_memoryPointer = 0;

void
BLSlave::st_header_received()
{
    switch (current_FrameID()) {
    case kFrameIDMasterRequest:
        st_expect_response();
        break;

    case kFrameIDSlaveResponse:
        if (_sendIndex != kNoSendResponse) {
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
    _sendIndex = kNoSendResponse;

    switch (current_FrameID()) {
    case kFrameIDMasterRequest:

        if (resp.MasterRequest.nad != nad()) {
            break;
        }

        switch (resp.MasterRequest.sid) {
        case service_id::kReadDataByID:
            _sendIndex = resp.DataByID.index;
            break;

        case service_id::kWriteDataByID:
            switch (resp.DataByID.index) {
            case Generic::kParamBootloaderMode:
                // reset the bootloader state
                if (resp.DataByID.value == 1) {
                    _pageStatus = bl_status::kWaitingForProgrammer;
                    _programEnd = 0;
                    _resetVector = 0;
                }
                break;

            case Bootloader::kParamPageAddress:
                set_page_address(resp.DataByID.value);
                break;

            case Bootloader::kParamPageCRC:

                // pad the buffer out with 0xff
                while (add_page_byte(0xff)) {
                }

                // if the CRC matches, program the page
                if (resp.DataByID.value == _runningCrc) {
                    program_page();
                    _pageStatus = bl_status::kReadyForPage;

                } else {
                    _pageStatus = bl_status::kPageCRCError;
                }

            case Bootloader::kParamDebugPointer:
                _memoryPointer = resp.DataByID.value;
                break;

            case Generic::kParamConfigBase ... Generic::kParamConfigTop:
                eeprom_update_word((uint16_t *)((_sendIndex - Generic::kParamConfigBase) * 2), resp.DataByID.value);
                break;

            }
            break;

        case service_id::kDataDump:

            // add bytes to the page buffer
            if (_pageStatus == bl_status::kPageInProgress) {
                uint8_t count = resp.MasterRequest.length - 1;
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
    uint16_t value = 0x0bad;
    bool invalid = false;

    switch (_sendIndex) {
    case Generic::kParamProtocolVersion:
        // XXX should come from the defs file
        value = 1;
        break;

    case Generic::kParamBoardFunction:
        value = function();
        break;

    case Generic::kParamBootloaderMode:
        value = 1;
        break;

    case Generic::kParamFirmwarePageSize:
        value = SPM_PAGESIZE;
        break;

    case Bootloader::kParamPageAddress:
        value = _pageAddress;
        break;

    case Bootloader::kParamPageOffset:
        value = _pageOffset;
        break;

    case Bootloader::kParamPageCRC:
        value = _runningCrc;
        break;

    case Bootloader::kParamDebugPointer:
        value = _memoryPointer;
        break;

    case Bootloader::kParamStatus:
        value = _pageStatus;
        break;

    case Bootloader::kParamMemory:
        value = pgm_read_byte((const uint8_t *)_memoryPointer);
        break;

    case Bootloader::kParamEEPROM:
        value = eeprom_read_byte((const uint8_t *)_memoryPointer);
        break;

    case Generic::kParamLine ... Generic::kParamProtocol:
        value = errors[_sendIndex - Generic::kParamLine];
        break;

    case Generic::kParamConfigBase ... Generic::kParamConfigTop:
        value = eeprom_read_word((uint16_t *)((_sendIndex - Generic::kParamConfigBase) * 2));
        break;

    default:
        invalid = true;
        break;
    }

    Response resp;
    resp.DataByID.nad = nad();
    resp.DataByID.pci = pci::kSingleFrame;
    if (!invalid) {
        resp.DataByID.length = 5;
        resp.DataByID.sid = (service_id::kReadDataByID | service_id::kResponseOffset);
        resp.DataByID.index = _sendIndex;
        resp.DataByID.value = value;
    } else {
        resp.ServiceError.length = 3;
        resp.ServiceError.sid = service_id::kErrorResponse;
        resp.ServiceError.original_sid = service_id::kReadDataByID;
        resp.ServiceError.error = service_error::kFunctionNotSupported;
    }

    _sendIndex = kNoSendResponse;

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
    // kill the current program info, since we are starting a new program
    if (_pageStatus == bl_status::kWaitingForProgrammer) {
        update_program_info();
    }

    // set the new page address and prepare for the page
    if ((_pageAddress < BL_ADDR) && !(_pageAddress % SPM_PAGESIZE)) {
        _pageAddress = address;
        _pageOffset = 0;
        _runningCrc = 0;
        _pageStatus = bl_status::kPageInProgress;
    } else {
        _pageStatus = bl_status::kPageAddressError;
    }
}

bool
BLSlave::add_page_byte(uint8_t byte)
{
    if (_pageOffset < SPM_PAGESIZE) {
        _runningCrc = _crc_xmodem_update(_runningCrc, byte);
        uint8_t *p = (uint8_t *)&_pageBuffer[0];
        p[_pageOffset++] = byte;
        return true;
    }

    return false;
}

void
BLSlave::program_page()
{
    // patch our reset vector into the buffer
    if (_pageAddress == 0) {
        if (_pageBuffer[0] == 0x940c) {
            // copy jmp argument
            _resetVector = _pageBuffer[1];

        } else {
            // extract rjmp destination
            _resetVector = ((_pageBuffer[0] & ~0xc000) + 1) * 2;
            // convert to jmp
            _pageBuffer[0] = 0x940c;
        }

        // patch in bootloader reset vector
        _pageBuffer[1] = BL_ADDR;
    }

    boot_page_erase(_pageAddress);
    boot_spm_busy_wait();

    for (uint8_t i = 0; i < (SPM_PAGESIZE / 2); i++) {
        boot_page_fill(_pageAddress + (i * 2), _pageBuffer[i]);
    }

    boot_page_write(_pageAddress);
    boot_spm_busy_wait();

    // track the end of the programmed area
    _pageAddress += SPM_PAGESIZE;

    if (_pageAddress > _programEnd) {
        _programEnd = _pageAddress;
    }
}

void
BLSlave::update_program_info()
{

    boot_page_erase(kInfoPage);
    boot_spm_busy_wait();

    boot_page_fill(kInfoPage + 0, get_program_crc(_programEnd));
    boot_page_fill(kInfoPage + 2, _resetVector);
    boot_page_fill(kInfoPage + 4, _programEnd);

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

uint8_t
BLSlave::nad()
{
    uint8_t value = eeprom_read_byte((uint8_t *)(E2END - 3));

    if (value == 0xff) {
        value = Bootloader::kNodeAddress;
    }
    return value;
}

uint8_t
BLSlave::function()
{
    uint8_t value = eeprom_read_byte((uint8_t *)(E2END - 2));    

    return value;
}
