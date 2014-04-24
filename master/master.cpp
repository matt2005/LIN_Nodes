
#include <avr/interrupt.h>

#include "lin_drv.h"

#include "board.h"
#include "master.h"

static Master *_master;

Master::Master() :
    SwitchSlave(LIN::kNADMaster),
    _eventTimer((Timer::Callback)Master::event, (Timer::Period)10)
{
    _master = this;
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

void
Master::event()
{
    LIN::FrameID fid = LIN::kFIDNone;
    static uint8_t eventIndex;

    // work out what header we should send now 
    do {
        switch (eventIndex++) {
        case 0:
        case 2:
        case 4:
        case 6:
            // send controls
            fid = LIN::kFIDControls;
            break;

        case 1:
        case 3:
        case 5:
        case 7:
            // read switches
            fid = LIN::kFIDAuxSwitches;
            break;

        case 8:
            // MasterRequest/SlaveResponse
            if (_master->_requestFrame != nullptr) {
                fid = LIN::kFIDMasterRequest;
            } else if (_master->_responseFrame != nullptr) {
                fid = LIN::kFIDSlaveResponse;
            }
            break;

        default:
            // wrap
            eventIndex = 0;
            break;
        }
    } while (fid == LIN::kFIDNone);

    // turn on the LIN driver
    Board::linCS(true);

    // and transmit the header
    lin_tx_header(LIN_2X, fid, 0);
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

void
Master::sleepRequested(SleepType type)
{
    // Don't do anything here yet.
}
