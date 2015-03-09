/*
 * Copyright (c) 2012-2015, Mike Smith, <msmith@purgatory.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * o Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/io.h>

#include "lin_slave.h"
#include "board.h"


Slave::Slave(uint8_t nad, bool polled) :
    LINDev(polled),
    _nad(nad),
    _sendSlaveResponse(false)
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
    uint8_t sid = resp.MasterRequest.sid;

    resp.ServiceError.nad = _nad;
    resp.ServiceError.pci = pci::kSingleFrame;
    resp.ServiceError.length = 3;
    resp.ServiceError.sid = service_id::kErrorResponse;
    resp.ServiceError.original_sid = sid;
    resp.ServiceError.error = err;
    resp.ServiceError.d3 = 0xff;
    resp.ServiceError.d4 = 0xff;
    resp.ServiceError.d5 = 0xff;
}
