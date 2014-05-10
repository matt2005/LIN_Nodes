
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "board.h"
#include "timer.h"

#include "slave.h"

bool
ProgrammerSlave::setParameter(uint8_t nad, uint8_t param, uint8_t value)
{
    cli();
    _nodeAddress = nad;
    _paramIndex = param;
    _paramValue = value;
    _state = kStateSetWaitRequest;
    sei();

    // wait 100ms for the transaction to complete
    Timestamp t;
    while (_state != kStateIdle) {
        wdt_reset();
        if (t.isOlderThan(300)) {
            debug("set timed out with state %u", _state);
            return false;
        }
    }
    return true;
}

bool
ProgrammerSlave::getParameter(uint8_t nad, uint8_t param, uint8_t &value)
{
    cli();
    _nodeAddress = nad;
    _paramIndex = param;
    _state = kStateGetWaitRequest;
    sei();

    // wait 100ms for the transaction to complete
    Timestamp t;
    while (_state != kStateGetComplete) {
        wdt_reset();
        if (t.isOlderThan(300)) {
            debug("get timed out with state %u", _state);
            return false;
        }        
        if (_state == kStateError) {
            debug("get failed due to error");
            return false;
        }        
    }
    value = _paramValue;
    _state = kStateIdle;
    return true;
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
        debug("unexpected response");
    }
}

void
ProgrammerSlave::responseSent()
{
    if (_state == kStateSetWaitSent) {
        _state = kStateIdle;
    }
}
