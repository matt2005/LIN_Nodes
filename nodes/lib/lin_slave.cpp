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
Slave::st_response_received(Response &resp)
{
    switch (current_FrameID()) {
    case kFrameIDMasterRequest:
        // check for broadcast sleep request
        if (resp.MasterRequest.nad == node_address::kSleep) {
            st_sleep_requested(kSleepTypeRequested);
            break;
        }
        // check for directly addressed or broadcast master request
        if ((resp.MasterRequest.nad == _nad) ||
            (resp.MasterRequest.nad == node_address::kBroadcast)) {
            if (st_master_request(resp)) {
                st_slave_response(resp);
            }
        }
        break;

    default:
        LINDev::st_response_received(resp);
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
Slave::st_master_request(Response &resp)
{
    bool reply = false;
    // ReadByID
    switch (resp.MasterRequest.sid) {

    case service_id::kReadDataByID: {
        if (resp.DataByID.length != 3) {
            st_error_response(resp, service_error::kIncorrectLength);
        } else {
            uint16_t value;

            // look to see if we handle this one...
            if (st_read_data(resp.DataByID.index, value)) {
                resp.DataByID.pci = pci::kSingleFrame;
                resp.DataByID.length = 5;
                resp.DataByID.value = value;
                resp.DataByID.sid |= service_id::kResponseOffset;
            } else {
                // generic error...
                st_error_response(resp, service_error::kOutOfRange);
            }
        }
        reply = true;
        break;
    }

    case service_id::kWriteDataByID: {
        if (resp.DataByID.length != 5) {
            st_error_response(resp, service_error::kIncorrectLength);
        } else {
            // see if we can handle this one
            if (st_write_data(resp.DataByID.index, resp.DataByID.value)) {
                resp.DataByID.sid |= service_id::kResponseOffset;
                resp.DataByID.pci = pci::kSingleFrame;
                resp.DataByID.length = 3;
            } else {
                // generic error...
                st_error_response(resp, service_error::kOutOfRange);
            }
        }
        reply = true;
        break;
    }

    case service_id::kReadByID: {
        switch (resp.MasterRequest.d1) {
        default:
            // send an error
            st_error_response(resp, service_error::kFunctionNotSupported);
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
    if (Generic::param_exists(address)) {
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
Slave::st_error_response(Response &resp, uint8_t err)
{
    resp.MasterRequest.pci = pci::kSingleFrame;
    resp.MasterRequest.length = 2;
    resp.MasterRequest.d1 = resp.MasterRequest.sid;
    resp.MasterRequest.sid = service_id::kErrorResponse;
    resp.MasterRequest.d2 = err;
    resp.MasterRequest.d3 = 0xff;
    resp.MasterRequest.d4 = 0xff;
    resp.MasterRequest.d5 = 0xff;
}
