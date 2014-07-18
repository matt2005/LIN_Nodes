
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "board.h"
#include "timer.h"

#include "slave.h"

bool
ProgrammerSlave::set_parameter(uint8_t nad, uint8_t param, uint8_t value)
{
    for (uint8_t tries = 0; tries < 3; tries++) {
        cli();
        _nodeAddress = nad;
        _paramIndex = param;
        _paramValue = value;
        _state = kStateSetWaitRequest;
        sei();

        // wait 100ms for the transaction to complete
        Timestamp t;

        while (!t.is_older_than(100)) {
            wdt_reset();

            if (_state == kStateIdle) {
                uint8_t readback;

                if (get_parameter(nad, param, readback) && (readback == value)) {
                    return true;

                } else {
                    debug("set: readback %u not %u", readback, value);
                }
            }
        }

        debug("set: timed out in state %u", _state);
    }

    debug("set: failed");
    return false;
}

bool
ProgrammerSlave::get_parameter(uint8_t nad, uint8_t param, uint8_t &value)
{
    for (uint8_t tries = 0; tries < 3; tries++) {
        cli();
        _nodeAddress = nad;
        _paramIndex = param;
        _state = kStateGetWaitRequest;
        sei();

        // wait 100ms for the transaction to complete
        Timestamp t;

        while (!t.is_older_than(100)) {
            wdt_reset();

            if (_state == kStateGetComplete) {
                value = _paramValue;
                _state = kStateIdle;
                return true;
            }

            if (_state == kStateError) {
                break;
            }
        }

        debug("get: timed out in state %u", _state);
    }

    return false;
}

void
ProgrammerSlave::st_header_received()
{
    switch (current_FrameID()) {
    case LIN::kFrameIDConfigRequest:
        if (_state == kStateSetWaitRequest) {
            LIN::ConfigFrame f;

            f.nad() = _nodeAddress;
            f.flavour() = LIN::kCFSetParam;
            f.param() = _paramIndex;
            f.value() = _paramValue;

            st_send_response(f, 8);
            _state = kStateSetWaitSent;

        } else if (_state == kStateGetWaitRequest) {
            LIN::ConfigFrame f;

            f.nad() = _nodeAddress;
            f.flavour() = LIN::kCFGetParam;
            f.param() = _paramIndex;

            st_send_response(f, 8);
            _state = kStateGetWaitResponse;

        } else if (_state == kStateGetWaitResponse) {

            // We were expecting a ConfigResponse frame,
            // most likely that the previous response frame did
            // not get a reply.
            _state = kStateError;

        } else if (!_suspended) {
            LIN::ConfigFrame f;

            f.nad() = 0;
            f.flavour() = LIN::kCFNop;

            st_send_response(f, 8);
        }

        break;

    case LIN::kFrameIDConfigResponse:
        if (_state == kStateGetWaitResponse) {
            st_expect_response(8);
        }

        break;

    default:
        Slave::st_header_received();
        break;
    }
}

void
ProgrammerSlave::st_response_received(LIN::Frame &frame)
{
    // slave responding to a parameter request?
    if ((current_FrameID() == LIN::kFrameIDConfigResponse) &&
        (_state == kStateGetWaitResponse)) {

        auto cf = reinterpret_cast<LIN::ConfigFrame &>(frame);

        // sanity-check the reply
        if ((cf.nad() != _nodeAddress) ||
            (cf.flavour() != LIN::kCFGetParam) ||
            (cf.param() != _paramIndex)) {
            _state = kStateError;

        } else {
            _paramValue = cf.value();
            _state = kStateGetComplete;
        }

    } else {
        Slave::st_response_received(frame);
    }
}

void
ProgrammerSlave::st_response_sent()
{
    if (_state == kStateSetWaitSent) {
        _state = kStateIdle;
    }
}

void
ProgrammerSlave::sleep_requested(SleepType type)
{
    // XXX never sleep
}

void
ProgrammerSlave::master_request(LIN::Frame &frame)
{
    Board::panic(6);

    switch (frame.sid()) {
    case LIN::kServiceTesterPresent:
        // always send a positive response
        frame.sid() |= LIN::kServiceIDResponseOffset;
        slave_response(frame);
        break;

    default:
        Slave::master_request(frame);
        break;
    }
}
