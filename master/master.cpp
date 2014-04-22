
#include <avr/interrupt.h>

#include "lin_drv.h"

#include "board.h"
#include "master.h"

Master::Master() :
    SwitchSlave(LIN::kNADMaster),
    _eventTimer((Timer::Callback)Event::nextEvent, (Timer::Period)10)
{
}

bool
Master::doRequest(LIN::Frame &frame)
{
    cli();
    _requestFrame = &frame;
    _responseFrame = nullptr;
    sei();

    return waitRequest();
}

bool
Master::doRequestResponse(LIN::Frame &frame)
{
    cli();
    _requestFrame = &frame;
    _responseFrame = &frame;
    sei();

    return waitRequest();    
}

bool
Master::waitRequest()
{
    // spin for 100ms waiting for the frame to be sent
    _requestTimer.setRemaining(100);
    while (!_requestTimer.didExpire()) {
        if ((_requestFrame == nullptr) &&
            (_responseFrame == nullptr)) {
            _requestTimer.setRemaining(0);
            return true;
        }
    }
    _requestFrame = nullptr;
    _responseFrame = nullptr;
    return false;
}

void
Master::headerReceived(LIN::FID fid)
{
    switch (fid) {
    case LIN::kFIDControls:
        break;

    case LIN::kFIDMasterRequest:
        // do slave processing first (may reset state, arrange to listen, etc)
        SwitchSlave::headerReceived(fid);

        // if we have a request to send, commit it to the wire
        if (_requestFrame != nullptr) {
            sendResponse(*_requestFrame, 8);
            _requestFrame = nullptr;
        }
        break;

    case LIN::kFIDSlaveResponse:
        // arrange to receive the response if a slave sends it
        requestResponse(8);

        // and do slave processing
        SwitchSlave::headerReceived(fid);
        break;

    default:
        SwitchSlave::headerReceived(fid);
        break;
    }
}

void
Master::responseReceived(LIN::FID fid, LIN::Frame &frame)
{
    switch (fid) {

    case LIN::kFIDSlaveResponse:
        // if we are expecting a response, copy it back
        if (_responseFrame != nullptr) {
            *_responseFrame = frame;
            _responseFrame = nullptr;
        }
        break;

    default:
        SwitchSlave::responseReceived(fid, frame);
        break;
    }
}
