
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "board.h"
#include "timer.h"

#include "slave.h"

bool
ProgrammerSlave::set_parameter(uint8_t nad, uint8_t param, uint8_t value)
{
    return set_data_by_id(nad, 0, param, value);
}

bool
ProgrammerSlave::set_data_by_id(uint8_t nad, uint8_t page, uint8_t index, uint16_t value)
{
    for (uint8_t tries = 0; tries < 3; tries++) {
        cli();
        _nodeAddress = nad;
        _paramPage = page;
        _paramIndex = index;
        _paramValue = value;
        _state = kStateSetParam;
        sei();

        if (!wait_complete()) {
            continue;
        }

        uint8_t readback = ~value;

        if (get_parameter(nad, index, readback) && (readback == value)) {
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
    uint16_t tmp;

    bool result = get_data_by_id(nad, 0, param, tmp);
    value = tmp & 0xff;
    return result;
}

bool
ProgrammerSlave::get_error_count(uint8_t nad, uint8_t err, uint16_t &count)
{
    return get_data_by_id(nad, 1, err, count);
}

bool
ProgrammerSlave::get_data_by_id(uint8_t nad, uint8_t page, uint8_t index, uint16_t &value)
{
    for (uint8_t tries = 0; tries < 3; tries++) {
        cli();
        _nodeAddress = nad;
        _paramPage = page;
        _paramIndex = index;
        _paramValue = 0;
        _state = kStateGetParam;
        sei();

        if (!wait_complete()) {
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
                                        _paramPage,
                                        _paramValue & 0xff,
                                        _paramValue << 8));
            _state = kStateIdle;
            break;

        case kStateGetParam:
            st_send_response(LIN::Frame(_nodeAddress,
                                        3,
                                        LIN::kServiceIDReadDataByID,
                                        _paramIndex,
                                        _paramPage));
            _state = kStateWaitParam;
            break;

        case kStateWaitParam:
            // we have missed (or it never arrived) the reply to
            // the ReadDataByID operation
            _state = kStateError;
            break;

        default:
            break;
        }

        break;

    case LIN::kFrameIDSlaveResponse:
        if (_state == kStateWaitParam) {
            st_expect_response();
            break;
        }

    // If this is the response phase for the Tester Present sniff, we need to let
    // the generic slave code handle sending our response. Otherwise, if this is
    // something we are interested in, we should have set up to expect the response
    // above.

    // FALLTHROUGH

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

        // is this a response to a current request?
        if ((_state == kStateWaitParam) &&
            (frame.nad() == _nodeAddress) &&
            (frame.sid() == (LIN::kServiceIDReadDataByID | LIN::kServiceIDResponseOffset))) {

            // sanity-check the response
            if ((frame.pci() != 5) ||
                (frame.d1() != _paramIndex) ||
                (frame.d2() != _paramPage)) {
                _state = kStateError;
                debug("get: frame err");

            } else {
                _paramValue = frame.d3() | (frame.d4() << 8);
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

        // send a positive response to a directly-addressed request
        // unless suspended
        if (!_suspended && (frame.nad() == LIN::kNodeAddressTester)) {
            frame.sid() |= LIN::kServiceIDResponseOffset;
            reply = true;
        }

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
        wdt_reset();

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

    debug("wait_complete: %2u timeout in %1u", _nodeAddress, _state);
    _state = kStateIdle;
    return false;
}
