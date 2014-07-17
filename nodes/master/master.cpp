
#include <avr/interrupt.h>
#include <avr/wdt.h>

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
    LIN::kFrameIDMasterRequest, // skipped if no work
    LIN::kFrameIDSlaveResponse, // skipped if no work
};

const uint8_t Master::_normalScheduleLength = sizeof(Master::_normalSchedule) / sizeof(LIN::FrameID);

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
};

const uint8_t Master::_configScheduleLength = sizeof(Master::_configSchedule) / sizeof(LIN::FrameID);

Master::Master() :
    _eventTimer((Timer::Callback)Master::event, this, 10),
    _eventIndex(0),
    _requestFrame(nullptr),
    _responseFrame(nullptr),
    _configParam(0),
    _sendConfigResponseHeader(false),
    _sendConfigResponseFrame(false),
    _sleepEnable(false),
    _sleepActive(false),
    _programmerMode(false)
{
#ifdef pinDebugStrobe
    pinDebugStrobe.clear();
    pinDebugStrobe.cfg_output();
#endif
}

void
Master::do_request_response(LIN::Frame &frame)
{
    // post the frame for the schedule to see, avoid races with the
    // schedule runner
    cli();
    _requestFrame = &frame;
    _responseFrame = &frame;
    sei();

    // wait for 2 cycles total (fatal if not completed by then)
    Timestamp t;

    while (!t.is_older_than(schedule_length() * 10 * 2)) {
        wdt_reset();

        if (_responseFrame == nullptr) {
            break;
        }
    }

    _requestFrame = nullptr;
    _responseFrame = nullptr;
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

        if (_eventIndex >= schedule_length()) {
            // we hit the end of the pattern
            _eventIndex = 0;

            // no programmer present, yes we can sleep
            if (!_programmerMode && _sleepEnable) {
                _sleepActive = true;
                return;
            }
        }

        fid = schedule_entry(_eventIndex++);

        switch (fid) {

        case LIN::kFrameIDConfigResponse:

            // don't send ConfigResponse unless we know we need to
            if (!_sendConfigResponseHeader) {
                continue;
            }

            _sendConfigResponseHeader = false;
            break;

        case LIN::kFrameIDMasterRequest:

            // only send while a request/response cycle is running
            if (_requestFrame == nullptr) {
                continue;
            }

            break;

        case LIN::kFrameIDSlaveResponse:

            // only send while a request/response cycle is running
            if (_responseFrame == nullptr) {
                continue;
            }

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

        // send the request if we have one
        if (_requestFrame != nullptr) {
            send_response(*_requestFrame, 8);
            _requestFrame = nullptr;

        } else {
            // kill any stale response waiter
            _responseFrame = nullptr;
        }

        break;

    case LIN::kFrameIDSlaveResponse:
        // arrange to receive the response
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
        if (_responseFrame != nullptr) {
            _responseFrame->copy(frame);
            _responseFrame = nullptr;
        }

        break;

    default:
        break;
    }
}

void
Master::handle_config_request(LIN::ConfigFrame &frame)
{
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
