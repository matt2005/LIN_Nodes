
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "lin_drv.h"

#include "board.h"
#include "master.h"

#include "param_master.h"

// Schedule used in normal operation
//
PROGMEM const LIN::FrameID Master::_schedule[] = {
    LIN::kFrameIDRelays,
    LIN::kFrameIDMasterRequest, // skipped if no work
    LIN::kFrameIDSlaveResponse, // skipped if no work
    LIN::kFrameIDConfigRequest, // skipped if no programmer
    LIN::kFrameIDConfigResponse,// skipped if no programmer
};

const uint8_t Master::_scheduleLength = sizeof(Master::_schedule) / sizeof(Master::_schedule[0]);

Master::Master() :
    _eventTimer((Timer::Callback)Master::event, this, 10),
    _eventIndex(0),
    _requestFrame(nullptr),
    _responseFrame(nullptr),
    _configParam(0),
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

    while (!t.is_older_than(_scheduleLength * 10 * 2)) {
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

    do {

        if (_eventIndex >= _scheduleLength) {
            // we hit the end of the pattern
            _eventIndex = 0;

            // safe point in the schedule to enable sleep
            if (_sleepEnable) {
                _sleepActive = true;
                return;
            }

#ifdef pinDebugStrobe
            // toggle the trigger signal for the first frame in each cycle
            pinDebugStrobe.toggle();
#endif

        }

        // if sleep is active and not in programmer mode, send nothing
        // XXX should actually stop the schedule in this mode
        if (!_programmerMode && _sleepActive) {
            Board::lin_CS(false);
            return;
        }

        fid = schedule_entry(_eventIndex++);

        switch (fid) {

        case LIN::kFrameIDConfigRequest:
        case LIN::kFrameIDConfigResponse:

            // only send in programmer mode
            if (!_programmerMode) {
                continue;
            }

            break;

        case LIN::kFrameIDMasterRequest:

            // skip unless we have a request frame
            if (_requestFrame == nullptr) {
                continue;
            }

            break;

        case LIN::kFrameIDSlaveResponse:

            // skip unless we want a response
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
        // collect any response from the programmer
        expect_response(8);
        break;

    case LIN::kFrameIDConfigResponse:
        // we may want to send the config response ourselves
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
        // may be a request for us
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
