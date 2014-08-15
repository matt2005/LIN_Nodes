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


Slave::Slave(uint8_t nad, bool polled) :
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
    case kFrameIDMasterRequest:
        _sendSlaveResponse = false;     // clear stale slave response
        st_expect_response();
        break;

    case kFrameIDSlaveResponse:
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
Slave::st_response_received(Response &frame)
{
    switch (current_FrameID()) {
    case kFrameIDMasterRequest:
        // check for broadcast sleep request
        if (Signal::nad(frame) == node_address::kSleep) {
            st_sleep_requested(kSleepTypeRequested);
            break;
        }
        // check for directly addressed or broadcast master request
        if ((Signal::nad(frame) == _nad) ||
            (Signal::nad(frame) == node_address::kBroadcast)) {
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
Slave::st_master_request(Response &frame)
{
    bool reply = false;
    // ReadByID
    switch (Signal::sid(frame)) {

    case service_id::kReadDataByID: {
        if (Signal::length(frame) != 3) {
            st_error_response(frame, service_error::kIncorrectLength);
        } else {
            uint16_t value;

            // look to see if we handle this one...
            if (st_read_data(Signal::index(frame), value)) {
                Signal::length(frame).set(5);
                Signal::value(frame).set(value);
                Signal::sid(frame).set(Signal::sid(frame) | service_id::kResponseOffset);
            } else {
                // generic error...
                st_error_response(frame, service_error::kOutOfRange);
            }
        }
        reply = true;
        break;
    }

    case service_id::kWriteDataByID: {
        if (Signal::length(frame) != 5) {
            st_error_response(frame, service_error::kIncorrectLength);
        } else {
            // see if we can handle this one
            if (st_write_data(Signal::index(frame), Signal::value(frame))) {
                Signal::sid(frame).set(Signal::sid(frame) | service_id::kResponseOffset);
                Signal::length(frame).set(3); // XXX assignment operator overload not working...
            } else {
                // generic error...
                st_error_response(frame, service_error::kOutOfRange);
            }
        }
        reply = true;
        break;
    }

    case service_id::kReadByID: {
        switch (Signal::d1(frame)) {
        default:
            // send an error
            st_error_response(frame, service_error::kFunctionNotSupported);
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
Slave::st_read_data(Parameter::Address address, uint16_t &value)
{
    // handle generic parameters that we explicitly know about here
    switch (address) {
    case Generic::kParamLine:
    case Generic::kParamChecksum:
    case Generic::kParamParity:
    case Generic::kParamFraming:
    case Generic::kParamSynch:
    case Generic::kParamProtocol:
        value = errors[address - Generic::kParamLine];
        return true;   
    }

    // handle generic parameters known elsewhere
    uint8_t encoding = Generic::param_encoding(address);
    if (encoding != kEncoding_none) {
        value = Parameter(address).get();
        return true;
    }
    return false;
}

bool
Slave::st_write_data(Parameter::Address address, uint16_t value)
{
    uint8_t encoding = Generic::param_encoding(address);
    if ((encoding != kEncoding_none) && !Encoding::invalid(encoding, value)) {
        Parameter(address).set(value);
        return true;
    }
    return false;
}

void
Slave::st_error_response(Response &frame, uint8_t err)
{
    Signal::length(frame).set(2);
    Signal::d1(frame).set(Signal::sid(frame));
    Signal::sid(frame).set(service_id::kErrorResponse);
    Signal::d2(frame).set(err);
    Signal::d3(frame).set(0xff);
    Signal::d4(frame).set(0xff);
    Signal::d5(frame).set(0xff);
}
