
#include <avr/interrupt.h>

#include "lin_drv.h"

#include "board.h"
#include "master.h"

// Schedule used in normal operation
//
PROGMEM const LIN::FrameID Master::normalSchedule[] = {
    LIN::kFIDRelays,
    LIN::kFIDRelays,
    LIN::kFIDRelays,
    LIN::kFIDRelays,
    LIN::kFIDConfigRequest, // sniff for the programmer
    LIN::kFIDRelays,
    LIN::kFIDRelays,
    LIN::kFIDRelays,
    LIN::kFIDRelays,
    LIN::kFIDMasterRequest, // skipped if no work
    LIN::kFIDSlaveResponse, // skipped if no work
    LIN::kFIDNone
};

// Schedule used when the programmer has been seen - should never
// be more than 50ms between the programmer being ready to send a
// request and the response being posted
//
PROGMEM const LIN::FrameID Master::configSchedule[] = {
    LIN::kFIDRelays,
    LIN::kFIDConfigRequest,
    LIN::kFIDConfigResponse,// skipped if no work
    LIN::kFIDRelays,
    LIN::kFIDConfigRequest,
    LIN::kFIDConfigResponse,// skipped if no work
    LIN::kFIDMasterRequest, // skipped if no work
    LIN::kFIDConfigRequest,
    LIN::kFIDConfigResponse,// skipped if no work
    LIN::kFIDSlaveResponse, // skipped if no work
    LIN::kFIDConfigRequest,
    LIN::kFIDConfigResponse,// skipped if no work
    LIN::kFIDNone
};

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
    LIN::FrameID fid;

    do {

        fid = (LIN::FrameID)pgm_read_byte((_configDecayTimer > 0) ? 
                                           &configSchedule[_eventIndex] :
                                           &normalSchedule[_eventIndex]);
        _eventIndex++;

        if (fid == LIN::kFIDNone) {
            if (_configDecayTimer > 0) {
                _configDecayTimer--;
            }
            _eventIndex = 0;
            continue;
        }
        if ((fid == LIN::kFIDConfigResponse) && !_sendConfigResponseHeader) {
            continue;
        }
        if ((fid == LIN::kFIDMasterRequest) && !_sendRequest) {
            continue;
        }
        if ((fid == LIN::kFIDSlaveResponse) && !_getResponse) {
            continue;
        }
    } while(0);

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
    while (!t.isOlderThan(100)) {
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

    case LIN::kFIDConfigRequest:
        requestResponse(8);
        break;

    case LIN::kFIDConfigResponse:
        handleConfigResponse();
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
    // reset the decay timer so that we stay on the config schedule
    _configDecayTimer = 20;

    // ignore this frame?
    if (frame.flavour() == LIN::kCFNop) {
        return;
    }

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
