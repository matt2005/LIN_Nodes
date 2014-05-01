
#include <avr/interrupt.h>

#include "lin_drv.h"

#include "board.h"
#include "master.h"

Master::Master() :
    Slave(LIN::kNADMaster),
    _eventTimer((Timer::Callback)Master::event, this, 10),
    _eventIndex(0)
{
}

bool
Master::doRequest(LIN::Frame &frame)
{
    cli();
    requestResponseFrame.copy(frame);
    _sendRequest = true;
    _getResponse = false;
    sei();

    return waitRequest();
}

bool
Master::doRequestResponse(LIN::Frame &frame)
{
    cli();
    requestResponseFrame.copy(frame);
    _sendRequest = true;
    _getResponse = true;
    sei();

    if (!waitRequest()) {
        return false;
    }
    frame.copy(requestResponseFrame);
    return true;
}

void
Master::event(void *arg)
{
    auto *master = (Master *)arg;

    master->_event();
}

void
Master::_event()
{
    LIN::FrameID fid = LIN::kFIDNone;

    // work out what header we should send now 
    do {
        switch (_eventIndex++) {
        case 0:
        case 2:
        case 4:
        case 6:
            // send controls
            fid = LIN::kFIDNonsense; //LIN::kFIDRelays;
            break;

        case 1:
        case 3:
        case 5:
        case 7:
            // read switches
            fid = LIN::kFIDNonsense; //LIN::kFIDAuxSwitches;
            break;

        case 8:
            // MasterRequest/SlaveResponse
            if (_sendRequest) {
                fid = LIN::kFIDMasterRequest;
            } else if (_getResponse) {
                fid = LIN::kFIDSlaveResponse;
            }
            break;

        default:
            // wrap
            _eventIndex = 0;
            break;
        }
    } while (fid == LIN::kFIDNone);

    // stubbed out noise packets
    if (fid == LIN::kFIDNonsense)
        return;

    // turn on the LIN driver
    Board::linCS(true);

    // and transmit the header
    sendHeader(fid);
}

bool
Master::waitRequest()
{
    auto then = Timer::timeNow();

    // spin for 100ms waiting for the frame to be sent
    while (Timer::timeSince(then) < 500) {
        if (!_sendRequest && !_getResponse) {
            return true;
        }
    }
    _sendRequest = false;
    _getResponse = false;
    return false;
}

void
Master::headerReceived(LIN::FID fid)
{
    switch (fid) {
    case LIN::kFIDRelays:
        sendResponse(relayFrame, 8);
        break;

    case LIN::kFIDMasterRequest:
        // if we have a request to send, commit it to the wire
        if (_sendRequest) {
            sendResponse(requestResponseFrame, 8);
            _sendRequest = false;
        }
        break;

    case LIN::kFIDSlaveResponse:
        // arrange to receive the response if a slave sends it
        requestResponse(8);

        break;

    default:
        break;
    }
}

void
Master::responseReceived(LIN::FID fid, LIN::Frame &frame)
{
    switch (fid) {

    case LIN::kFIDSlaveResponse:
        // if we are expecting a response, copy it back
        if (_getResponse) {
            requestResponseFrame.copy(frame);
            _getResponse = false;
        }
        break;

    default:
        break;
    }
}
