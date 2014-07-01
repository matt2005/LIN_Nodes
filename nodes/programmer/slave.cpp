
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "board.h"
#include "timer.h"

#include "slave.h"

bool
ProgrammerSlave::setParameter(uint8_t nad, uint8_t param, uint8_t value)
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

        while (!t.isOlderThan(100)) {
            wdt_reset();

            if (_state == kStateIdle) {
                uint8_t readback;

                if (getParameter(nad, param, readback) && (readback == value)) {
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
ProgrammerSlave::getParameter(uint8_t nad, uint8_t param, uint8_t &value)
{
    for (uint8_t tries = 0; tries < 3; tries++) {
        cli();
        _nodeAddress = nad;
        _paramIndex = param;
        _state = kStateGetWaitRequest;
        sei();

        // wait 100ms for the transaction to complete
        Timestamp t;

        while (!t.isOlderThan(100)) {
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
ProgrammerSlave::headerReceived(LIN::FID fid)
{
    switch (fid) {
    case LIN::kFIDConfigRequest:
        if (_state == kStateSetWaitRequest) {
            LIN::ConfigFrame f;

            f.nad() = _nodeAddress;
            f.flavour() = LIN::kCFSetParam;
            f.param() = _paramIndex;
            f.value() = _paramValue;

            sendResponse(f, 8);
            _state = kStateSetWaitSent;

        } else if (_state == kStateGetWaitRequest) {
            LIN::ConfigFrame f;

            f.nad() = _nodeAddress;
            f.flavour() = LIN::kCFGetParam;
            f.param() = _paramIndex;

            sendResponse(f, 8);
            _state = kStateGetWaitResponse;

        } else if (!_suspended) {
            LIN::ConfigFrame f;

            f.nad() = 0;
            f.flavour() = LIN::kCFNop;

            sendResponse(f, 8);
        }

        break;

    case LIN::kFIDConfigResponse:
        if (_state == kStateGetWaitResponse) {
            requestResponse(8);
        }

        break;

    default:
        break;
    }
}

void
ProgrammerSlave::responseReceived(LIN::FID fid, LIN::Frame &frame)
{
    // slave responding to a parameter request?
    if ((fid == LIN::kFIDConfigResponse) &&
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
        debug("unexpected response %u in state %u", fid, _state);
    }
}

void
ProgrammerSlave::responseSent()
{
    if (_state == kStateSetWaitSent) {
        _state = kStateIdle;
    }
}

void
ProgrammerSlave::sleepRequested(SleepType type)
{
    // XXX never sleep
}
