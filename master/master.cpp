
#include <avr/interrupt.h>

#include "lin_drv.h"

#include "board.h"
#include "master.h"

Master::Master() :
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
        case 0 ... 3:
            fid = LIN::kFIDRelays;
            break;

        case 4:
            // XXX reduce rate if programmer not present
            fid = LIN::kFIDConfigRequest;
            break;

        case 5 ... 8:
            fid = LIN::kFIDRelays;
            break;

        case 9:
            if (_sendConfigResponseHeader) {
                _sendConfigResponseHeader = false;
                fid = LIN::kFIDConfigResponse;
            }
            break;

        case 10 ... 13:
            fid = LIN::kFIDRelays;
            break;

        case 14:
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
    // spin for 100ms waiting for the frame to be sent
    Timestamp t;
    while (!t.olderThan(100)) {
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

    case LIN::kFIDConfigResponse:
        handleConfigResponse();

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

    case LIN::kFIDConfigRequest:
        handleConfigRequest(reinterpret_cast<LIN::ConfigFrame &>(frame));
        break;

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

void
Master::handleConfigRequest(LIN::ConfigFrame &frame)
{
    // we will want to send a ConfigResponse header later
    _sendConfigResponseHeader = true;

    if (frame.nad() != LIN::kNADMaster) {
        return;
    }
    if (frame.flavour() == LIN::kCFGetParam) {
        _configParam = frame.param();
        _sendConfigResponseFrame = true;
        return;
    }
    if (frame.flavour() == LIN::kCFSetParam) {
        Parameter(frame.param()).set(frame.value());
        return;
    }
}

void
Master::handleConfigResponse()
{
    if (!_sendConfigResponseFrame) {
        return;
    }
    _sendConfigResponseFrame = false;

    LIN::ConfigFrame f;

    f.nad() = LIN::kNADMaster;
    f.flavour() = LIN::kCFGetParam;
    f.param() = _configParam;
    f.value() = Parameter(_configParam).get();

    sendResponse(f, 8);
}
