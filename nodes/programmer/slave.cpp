
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
        _state = kStateSetParam;
        sei();

        if (!wait_complete()) {
            debug("set: failed to set");
            continue;
        }

        uint8_t readback = ~value;

        if (get_parameter(nad, param, readback) && (readback == value)) {
            return true;
        }

        debug("set: readback %u not %u", readback, value);
    }

    debug("set: failed after retries");
    return false;
}

bool
ProgrammerSlave::get_parameter(uint8_t nad, uint8_t param, uint8_t &value)
{
    for (uint8_t tries = 0; tries < 3; tries++) {
        cli();
        _nodeAddress = nad;
        _paramIndex = param;
        _paramValue = 0;
        _state = kStateGetParam;
        sei();

        if (!wait_complete()) {
            debug("get: failed to get");
            continue;
        }

        value = _paramValue;
        return true;
    }

    debug("get: failed after retries");
    return false;
}

void
ProgrammerSlave::st_header_received()
{
    switch (current_FrameID()) {
    case LIN::kFrameIDProxyRequest:
        switch (_state) {
        case kStateSetParam:
            st_send_response(LIN::Frame(_nodeAddress,
                                        5,
                                        LIN::kServiceIDWriteDataByID,
                                        _paramIndex,
                                        0,
                                        _paramValue,
                                        0));
            _state = kStateIdle;
            break;

        case kStateGetParam:
            st_send_response(LIN::Frame(_nodeAddress,
                                        5,
                                        LIN::kServiceIDReadDataByID,
                                        _paramIndex,
                                        0));
            _state = kStateWaitParam;
            break;

        default:
            break;
        }

        break;

    case LIN::kFrameIDSlaveResponse:
        if (_state == kStateWaitParam) {
            st_expect_response();
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
    switch (current_FrameID()) {
    case LIN::kFrameIDSlaveResponse:
        if (_state == kStateWaitParam) {
            if ((frame.nad() != _nodeAddress) ||
                (frame.pci() != 5) ||
                (frame.sid() != (LIN::kServiceIDReadDataByID | LIN::kServiceIDResponseOffset)) ||
                (frame.d1() != _paramIndex)) {
                _state = kStateError;

            } else {
                _paramValue = frame.d3();
                _state = kStateIdle;
            }
        }

        break;

    default:
        break;
    }

    Slave::st_response_received(frame);
}

void
ProgrammerSlave::sleep_requested(SleepType type)
{
    // XXX never sleep
}

bool
ProgrammerSlave::master_request(LIN::Frame &frame)
{
    bool reply = false;

    switch (frame.sid()) {
    case LIN::kServiceIDTesterPresent:
        // always send a positive response
        frame.sid() |= LIN::kServiceIDResponseOffset;
        reply = true;
        break;

    default:
        // do nothing here - we don't respond like a normal slave
        break;
    }

    return reply;
}

bool
ProgrammerSlave::wait_complete()
{
    Timestamp t;

    while (!t.is_older_than(100)) {
        switch (_state) {
        case kStateIdle:
            return true;

        case kStateError:
            debug("wait_complete: error");
            _state = kStateIdle;
            return false;

        default:
            break;
        }
    }

    debug("wait_complete: timeout");
    return false;
}
