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

#include "board.h"
#include "timer.h"

#include "slave.h"

bool
ProgrammerSlave::set_parameter(uint8_t nad, uint8_t param, uint8_t value)
{
    return set_data_by_id(nad, kDataPageNodeParameters, param, value);
}

bool
ProgrammerSlave::set_data_by_id(uint8_t nad, uint8_t page, uint8_t index, uint16_t value)
{
    for (uint8_t tries = 0; tries < 3; tries++) {
        cli();
        _nodeAddress = nad;
        _dataPage = page;
        _dataIndex = index;
        _dataValue = value;
        _state = kStateSetData;
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

    bool result = get_data_by_id(nad, kDataPageNodeParameters, param, tmp);
    value = tmp & 0xff;
    return result;
}

bool
ProgrammerSlave::get_error_count(uint8_t nad, uint8_t err, uint16_t &count)
{
    return get_data_by_id(nad, kDataPageLINErrors, err, count);
}

bool
ProgrammerSlave::get_data_by_id(uint8_t nad, uint8_t page, uint8_t index, uint16_t &value)
{
    for (uint8_t tries = 0; tries < 3; tries++) {
        cli();
        _nodeAddress = nad;
        _dataPage = page;
        _dataIndex = index;
        _dataValue = 0;
        _state = kStateGetData;
        sei();

        if (!wait_complete()) {
            continue;
        }

        value = _dataValue;
        return true;
    }

    debug("get: failed after retries");
    return false;
}

void
ProgrammerSlave::st_header_received()
{
    switch (current_FrameID()) {
    case kFrameIDProxyRequest:
        switch (_state) {
        case kStateSetData:
            st_send_response(Response(_nodeAddress,
                                        5,
                                        service_id::kWriteDataByID,
                                        _dataIndex,
                                        _dataPage,
                                        _dataValue & 0xff,
                                        _dataValue << 8));
            _state = kStateIdle;
            break;

        case kStateGetData:
            st_send_response(Response(_nodeAddress,
                                        3,
                                        service_id::kReadDataByID,
                                        _dataIndex,
                                        _dataPage));
            _state = kStateWaitData;
            break;

        case kStateWaitData:
            // we have missed (or it never arrived) the reply to
            // the ReadDataByID operation
            _state = kStateError;
            break;

        default:
            break;
        }

        break;

    case kFrameIDSlaveResponse:

        // are we expecting someone else to be sending a response?
        if (_state == kStateWaitData) {
            st_expect_response();
            break;
        }

    // no - maybe we are expected to send the response?
    // FALLTHROUGH

    default:
        Slave::st_header_received();
        break;
    }
}

void
ProgrammerSlave::st_response_received(Response &frame)
{
    switch (current_FrameID()) {
    case kFrameIDSlaveResponse:

        // is this a response to a current request?
        if ((_state == kStateWaitData) &&
            (frame.nad() == _nodeAddress) &&
            (frame.sid() == (service_id::kReadDataByID | service_id::kResponseOffset))) {

            // sanity-check the response
            if ((frame.pci() != 5) ||
                (frame.d1() != _dataIndex) ||
                (frame.d2() != _dataPage)) {
                _state = kStateError;
                debug("get: frame err");

            } else {
                _dataValue = frame.d3() | (frame.d4() << 8);
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
ProgrammerSlave::st_sleep_requested(SleepType type)
{
    // XXX never sleep
}

bool
ProgrammerSlave::st_master_request(Response &frame)
{
    bool reply = false;

    switch (frame.sid()) {
    case service_id::kTesterPresent:

        // send a positive response to a directly-addressed request
        // unless suspended
        if (!_suspended && (frame.nad() == LIN::kNodeAddressTester)) {
            frame.sid() |= service_id::kResponseOffset;
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
