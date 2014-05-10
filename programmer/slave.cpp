
#include <avr/interrupt.h>

#include "slave.h"

void
ProgrammerSlave::setParameter(LIN::NodeAddress nad, uint8_t param, uint8_t value)
{
    cli();
    _nodeAddress = nad;
    _paramIndex = param;
    _paramValue = value;
    _state = kStateSetWaitRequest;
    sei();
}

void
ProgrammerSlave::requestParameter(LIN::NodeAddress nad, uint8_t param)
{
    cli();
    _nodeAddress = nad;
    _paramIndex = param;
    _state = kStateGetWaitRequest;
    sei();
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
            _state = kStateGetWaitSlaveResponse;
        }
        break;

    case LIN::kFIDConfigResponse:
        if (_state == kStateGetWaitSlaveResponse) {
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
        (_state == kStateGetWaitSlaveResponse)) {

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
    }
}

void
ProgrammerSlave::responseSent()
{
    if (_state == kStateSetWaitSent) {
        _state = kStateIdle;
    }
}
