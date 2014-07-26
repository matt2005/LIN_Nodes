/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/io.h>

#include "lin_slave.h"
#include "board.h"


Slave::Slave(LIN::NodeAddress nad, bool polled) :
    LINDev(polled),
    _nad(nad)
{
}

void
Slave::tick()
{
    // do any low-level work first
    LINDev::tick();

    // ... and if we're still idle and timed out, go to sleep
    if (_lastActivity.is_older_than(4000)) {
        st_sleep_requested(kSleepTypeIdle);
    }
}

void
Slave::st_header_received()
{
    switch (current_FrameID()) {
    case LIN::kFrameIDMasterRequest:
        _sendSlaveResponse = false;     // clear stale slave response
        st_expect_response();
        break;

    case LIN::kFrameIDSlaveResponse:
        if (_sendSlaveResponse) {
            st_send_response(_response);
            _sendSlaveResponse = false;
        }
        break;

    default:
        LINDev::st_header_received();
    }

    // reset the idle timeout
    _lastActivity.update();
}

void
Slave::st_response_received(LIN::Frame &frame)
{
    switch (current_FrameID()) {
    case LIN::kFrameIDMasterRequest:
        // check for broadcast sleep request
        if (frame.nad() == LIN::kNodeAddressSleep) {
            st_sleep_requested(kSleepTypeRequested);
            break;
        }
        // check for directly addressed or broadcast master request
        if ((frame.nad() == _nad) ||
            (frame.nad() == LIN::kNodeAddressBroadcast)) {
            if (st_master_request(frame)) {
                st_slave_response(frame);
            }
        }
        break;

    default:
        LINDev::st_response_received(frame);
        break;
    }
}

void
Slave::st_sleep_requested(SleepType type)
{
    // default behaviour is to behave as requested
    Board::sleep();
}

bool
Slave::st_master_request(LIN::Frame &frame)
{
    bool reply = false;
    // ReadByID
    switch (frame.sid()) {

    case LIN::kServiceIDReadDataByID: {
        if (frame.pci() != 3) {
            frame.set_error(LIN::kServiceErrorIncorrectLength);
        } else {
            uint16_t value;

            // look to see if we handle this one...
            if (st_read_data(frame.d2(), frame.d1(), value)) {
                frame.pci() = 5;
                frame.sid() |= LIN::kServiceIDResponseOffset;
            } else {
                // generic error...
                frame.set_error(LIN::kServiceErrorOutOfRange);
            }
        }
        reply = true;
        break;
    }

    case LIN::kServiceIDWriteDataByID: {
        if (frame.pci() != 5) {
            frame.set_error(LIN::kServiceErrorIncorrectLength);
        } else {
            uint16_t value = ((uint16_t)frame.d4() << 8) | frame.d3();

            // see if we can handle this one
            if (st_write_data(frame.d2(), frame.d1(), value)) {
                frame.pci() = 3;
                frame.sid() |= LIN::kServiceIDResponseOffset;
            } else {
                // generic error...
                frame.set_error(LIN::kServiceErrorOutOfRange);
            }
        }
        reply = true;
        break;
    }

    case LIN::kServiceIDReadByID: {
        switch (frame.d1()) {
        case LIN::kReadByIDProductID:
            frame.pci() = 6;
            frame.sid() |= LIN::kServiceIDResponseOffset;
            frame.d1() = LIN::kSupplierID & 0xff;
            frame.d2() = LIN::kSupplierID >> 8;
            frame.d3() = kBoardFunctionID;
            frame.d4() = Board::get_mode();
            frame.d5() = 0;
            reply = true;
            break;

        default:
            // send an error
            frame.set_error(LIN::kServiceErrorFunctionNotSupported);
            reply = true;
            break;
        }
        break;
    }
    }

    return reply;
}

static const PROGMEM uint16_t page0[] = 
{
    1,                              // XXX protocol version
    (uint16_t)kBoardFunctionID,
    0,                              // bootloader mode (not)
    RELEASE_VERSION,
    SPM_PAGESIZE,
};

bool
Slave::st_read_data(uint8_t page, uint8_t index, uint16_t &value)
{
    bool result = false;

    switch (page) {
    case kDataPageInfo:
        if (index < (sizeof(page0) / sizeof(page0[0]))) {
            value = pgm_read_word(&page0[index]);
            result = true;
        }
        break;
    case kDataPageLINErrors:
        if (index < kLINErrorMax) {
            value = errors[index];
            result = true;
        }
    default:
        break;
    }

    return result;
}

bool
Slave::st_write_data(uint8_t page, uint8_t index, uint16_t value)
{
    // 
    if ((page == kDataPageInfo) && 
        (index == kNodeInfoBootloaderMode) &&
        (value == 0x4f42)) {        // 'BO'
        Board::enter_bootloader();
    }

    return false;
}
