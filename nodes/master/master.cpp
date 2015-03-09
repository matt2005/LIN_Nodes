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

#include "lin_drv.h"

#include "board.h"
#include "master.h"

// Schedule used in normal operation
//
PROGMEM const uint8_t MasterNode::_schedule[] = {
    kFrameIDRelays,
    kFrameIDMasterRequest,
    kFrameIDSlaveResponse,
};

const uint8_t MasterNode::_scheduleLength = sizeof(MasterNode::_schedule) / sizeof(MasterNode::_schedule[0]);

MasterNode::MasterNode() :
    Slave(Master::kNodeAddress),
    _testerPresent(0),
    _mtTimer((Timer::Callback)MasterNode::master_task, this, _frameTime),
    _mtIndex(0),
    _mtAwake(true),
    _mtSleepRequest(false),
    _stExpectResponse(false)
{
#ifdef pinDebugStrobe
    pinDebugStrobe.clear();
    pinDebugStrobe.cfg_output();
#endif
}

void
MasterNode::master_task(void *arg)
{
    reinterpret_cast<MasterNode *>(arg)->_master_task();
}

void
MasterNode::_master_task()
{
    // If the master task is not awake, don't do anything here.
    if (!_mtAwake) {
        return;
    }

    // If there was a tester attached, and it is still active, don't
    // do anything here.
    if (_testerPresent) {
        if (_lastActivity.is_older_than(2000U)) {
            // assume the tester has been disconnected or idled
            _testerPresent = false;
        }

        return;
    }

    // walk along the schedule looking for something to do
    do {

        if (_mtIndex >= _scheduleLength) {
            // we hit the end of the pattern
            _mtIndex = 0;

            // safe point in the schedule to enable sleep, if requested and allowed
            if (_mtSleepRequest) {
                _mtAwake = false;
                // turn off the transceiver
                Board::lin_CS(false);
                return;
            }

#ifdef pinDebugStrobe
            // toggle the trigger signal for the first frame in each cycle
            pinDebugStrobe.toggle();
#endif

        }

        // get the next frame from the schedule
        uint8_t fid = schedule_entry(_mtIndex++);

        // special handling for optional frame types
        switch (fid) {

        case kFrameIDMasterRequest:

            // XXX optimise - only send when we have something to send

            break;

        case kFrameIDSlaveResponse:

            // only send when something is expected to respond
            if (!_stExpectResponse) {
                continue;
            }

            _stExpectResponse = false;

            break;

        default:
            // send the frame as requested
            break;
        }

        // and transmit the header
        mt_send_header(fid);

        break;
    } while (1);
}

void
MasterNode::st_header_received()
{
    // if we are currently the master, and awake...
    if (_mtAwake && !_testerPresent) {
        Response resp;

        switch (current_FrameID()) {
        case kFrameIDRelays:
            // send the relay state
            st_send_response(relayFrame);
            break;

        case kFrameIDMasterRequest:

            // XXX should be more flexible about what we send here...

            resp.SlaveResponse.nad = Tester::kNodeAddress;
            resp.SlaveResponse.pci = pci::kSingleFrame;
            resp.SlaveResponse.length = 2;
            resp.SlaveResponse.sid = service_id::kTesterPresent;
            resp.SlaveResponse.d1 = 0;
            st_send_response(resp);

            // make sure that we emit a SlaveResponse frame so that the slave can respond
            _stExpectResponse = true;
            break;

        case kFrameIDSlaveResponse:

            // arrange to receive the response for our own use
            st_expect_response();

            break;

        default:
            LINDev::st_header_received();
            break;
        }

        // reset the idle timeout
        _lastActivity.update();

    } else {
        // let the slave logic handle it
        Slave::st_header_received();
    }
}

bool
MasterNode::st_master_request(Response &resp)
{
    bool reply = false;

    // ReadByID
    switch (resp.MasterRequest.sid) {

    case service_id::kReadByID:
        switch (resp.MasterRequest.d1) {
        case 0: // LIN product identification
            resp.ReadByID.pci = pci::kSingleFrame;
            resp.ReadByID.length = 6;
            resp.ReadByID.sid |= service_id::kResponseOffset;
            resp.ReadByID.vendor = Master::kNodeSupplier;
            resp.ReadByID.function = Master::kNodeFunction;
            resp.ReadByID.variant = Master::kNodeVariant;
            reply = true;
            break;
        }
    }

    if (reply == false) {
        reply = Slave::st_master_request(resp);
    }

    return reply;
}

void
MasterNode::st_response_received(Response &resp)
{
    // if we are currently the master, and awake...
    if (_mtAwake && !_testerPresent) {

        switch (current_FrameID()) {

        case kFrameIDSlaveResponse:

            // is this a response from a tester?
            if ((resp.SlaveResponse.nad == Tester::kNodeAddress) &&
                (resp.SlaveResponse.sid == (service_id::kTesterPresent | service_id::kResponseOffset))) {
                _testerPresent = true;
            }

            break;

        default:
            // intentional bypass of Slave::st_response_received
            LINDev::st_response_received(resp);
            break;
        }

    } else {
        // let the slave logic handle it
        Slave::st_response_received(resp);
    }
}

void
MasterNode::st_sleep_requested(SleepType type)
{
    // ignore this
}

bool
MasterNode::st_read_data(Parameter::Address address, uint16_t &value)
{
    if (Master::param_exists(address)) {
        value = Parameter(address).get();
        return true;
    }

    // pass to superclass
    return Slave::st_read_data(address, value);
}

bool
MasterNode::st_write_data(Parameter::Address address, uint16_t value)
{
    uint8_t encoding = Master::param_encoding(address);

    if ((encoding != kEncoding_none) && !Encoding::invalid(encoding, value)) {
        Parameter(address).set(value);
        return true;
    }

    // pass to superclass
    return Slave::st_write_data(address, value);
}
