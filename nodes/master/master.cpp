
#include <avr/interrupt.h>

#include "lin_drv.h"

#include "board.h"
#include "master.h"

#include "param_master.h"

// Schedule used in normal operation
//
PROGMEM const LIN::FrameID Master::_normalSchedule[] = {
    LIN::kFrameIDRelays,
    LIN::kFrameIDRelays,
    LIN::kFrameIDRelays,
    LIN::kFrameIDRelays,
    LIN::kFrameIDConfigRequest, // sniff for the programmer
    LIN::kFrameIDRelays,
    LIN::kFrameIDRelays,
    LIN::kFrameIDRelays,
    LIN::kFrameIDRelays,
    LIN::kFrameIDMasterRequest, // skipped if no work
    LIN::kFrameIDSlaveResponse, // skipped if no work
    LIN::kFrameIDNone
};

// Schedule used when the programmer has been seen - should never
// be more than 50ms between the programmer being ready to send a
// request and the response being posted
//
// Note that master request/slave response and config request/response
// must not overlap, as slaves only have a single response buffer for each.
//
PROGMEM const LIN::FrameID Master::_configSchedule[] = {
    LIN::kFrameIDRelays,
    LIN::kFrameIDConfigRequest,
    LIN::kFrameIDConfigResponse,// skipped if no work
    LIN::kFrameIDMasterRequest, // skipped if no work
    LIN::kFrameIDSlaveResponse, // skipped if no work
    LIN::kFrameIDNone
};

Master::Master() :
    _eventTimer((Timer::Callback)Master::event, this, 10),
    _eventIndex(0),
    _configParam(0),
    _programmerGraceTimer(0),
    _sendRequest(false),
    _getResponse(false),
    _sendConfigResponseHeader(false),
    _sendConfigResponseFrame(false),
    _sleepEnable(false),
    _sleepActive(false)
{
#ifdef pinDebugStrobe
    pinDebugStrobe.clear();
    pinDebugStrobe.cfg_output();
#endif
}

bool
Master::do_request(LIN::Frame &frame)
{
    cli();
    requestResponseFrame.copy(frame);
    _sendRequest = true;
    _getResponse = false;
    sei();

    return wait_request();
}

bool
Master::do_request_response(LIN::Frame &frame)
{
    cli();
    requestResponseFrame.copy(frame);
    _sendRequest = true;
    _getResponse = true;
    sei();

    if (!wait_request()) {
        return false;
    }

    frame.copy(requestResponseFrame);
    return true;
}

void
Master::event(void *arg)
{
    auto *master = reinterpret_cast<Master *>(arg);

    master->_event();
}

void
Master::_event()
{
    LIN::FrameID fid;

    // if sleep is active, send nothing
    if (_sleepActive) {
        Board::lin_CS(false);
        return;
    }

    do {

        fid = (LIN::FrameID)pgm_read_byte((_programmerGraceTimer > 0) ?
                                          &_configSchedule[_eventIndex] :
                                          &_normalSchedule[_eventIndex]);
        _eventIndex++;

        switch (fid) {

        case LIN::kFrameIDNone:
            // we hit the end of the pattern
            _eventIndex = 0;

            // no programmer present, yes we can sleep
            if ((_programmerGraceTimer == 0) && _sleepEnable) {
                _sleepActive = true;
                return;
            }

            // give the programmer some time to respond before dropping back
            // to the regular schedule
            if (_programmerGraceTimer > 0) {
                _programmerGraceTimer--;
            }

            // go back and try the first frame
            continue;

        case LIN::kFrameIDConfigResponse:

            // don't send ConfigResponse unless we know we need to
            if (!_sendConfigResponseHeader) {
                continue;
            }

            _sendConfigResponseHeader = false;
            break;

        case LIN::kFrameIDMasterRequest:

            // don't send MasterRequest unless we know we need to
            if (!_sendRequest) {
                continue;
            }

            _sendRequest = false;
            break;

        case LIN::kFrameIDSlaveResponse:

            // don't send SlaveResponse unless we have someone waiting
            if (!_getResponse) {
                continue;
            }

            _getResponse = false;
            break;

        default:
            // send the frame as requested
            break;
        }

        break;
    } while (1);


    // turn on the LIN driver
    Board::lin_CS(true);

    // toggle post the trigger signal for the first frame in each cycle
#ifdef pinDebugStrobe

    if (_eventIndex == 1) {
        pinDebugStrobe.toggle();
    }

#endif

    // and transmit the header
    send_header(fid);
}

bool
Master::wait_request()
{
    // spin for 100ms waiting for the frame to be sent
    Timestamp t;

    while (!t.is_older_than(100)) {
        if (!_sendRequest && !_getResponse) {
            return true;
        }
    }

    _sendRequest = false;
    _getResponse = false;
    return false;
}

void
Master::header_received(LIN::FrameID fid)
{
    switch (fid) {
    case LIN::kFrameIDRelays:
        // send the relay state
        send_response(relayFrame, 8);
        break;

    case LIN::kFrameIDConfigRequest:
        // if the programmer is attached, it will send the response
        expect_response(8);
        break;

    case LIN::kFrameIDConfigResponse:
        // we may want to send the config response ourselves...
        handle_config_response();
        break;

    case LIN::kFrameIDMasterRequest:

        // if we have a request to send, commit it to the wire
        if (_sendRequest) {
            send_response(requestResponseFrame, 8);
            _sendRequest = false;
        }

        break;

    case LIN::kFrameIDSlaveResponse:
        // arrange to receive the response if a slave sends it
        expect_response(8);
        break;

    default:
        break;
    }
}

void
Master::response_received(LIN::FrameID fid, LIN::Frame &frame)
{
    switch (fid) {

    case LIN::kFrameIDConfigRequest:
        handle_config_request(reinterpret_cast<LIN::ConfigFrame &>(frame));
        break;

    case LIN::kFrameIDSlaveResponse:

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
Master::handle_config_request(LIN::ConfigFrame &frame)
{
    // reset the decay timer so that we stay on the config schedule
    _programmerGraceTimer = 0xff;

    // ignore this frame?
    if (frame.flavour() == LIN::kCFNop) {
        return;
    }

    // we will want to send a ConfigResponse header later
    _sendConfigResponseHeader = true;

    // if this request is for us...
    if (frame.nad() == LIN::kNodeAddressMaster) {

        // request for a parameter from us
        if (frame.flavour() == LIN::kCFGetParam) {
            _configParam = frame.param();
            _sendConfigResponseFrame = true;
        }

        // request to set a parameter
        if (frame.flavour() == LIN::kCFSetParam) {
            // XXX should range-check parameter index and value here...
            masterParam(frame.param()).set(frame.value());
        }
    }
}

void
Master::handle_config_response()
{
    if (!_sendConfigResponseFrame) {
        return;
    }

    _sendConfigResponseFrame = false;

    LIN::ConfigFrame f;

    f.nad() = LIN::kNodeAddressMaster;
    f.flavour() = LIN::kCFGetParam;
    f.param() = _configParam;
    f.value() = masterParam(_configParam);

    send_response(f, 8);
}
