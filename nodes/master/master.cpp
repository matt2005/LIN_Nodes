
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
    LIN::kFrameIDConfigRequest, // skipped if no tester
    LIN::kFrameIDConfigResponse,// skipped if no tester
};

const uint8_t Master::_scheduleLength = sizeof(Master::_schedule) / sizeof(Master::_schedule[0]);

Master::Master() :
    _mtTimer((Timer::Callback)Master::master_task, this, _frameTime),
    _mtIndex(0),
    _mtAwake(true),
    _mtSleepRequest(false),
    _requestFrame(nullptr),
    _responseFrame(nullptr),
    _configParam(0),
    _sendConfigResponseFrame(false),
    _testerPresent(false)
{
#ifdef pinDebugStrobe
    pinDebugStrobe.clear();
    pinDebugStrobe.cfg_output();
#endif
}

void
Master::do_request_response(LIN::Frame &frame)
{
    // ignored if the network is not awake
    if (!_mtAwake) {
        return;
    }

    // Post the frame for the schedule to see, avoid races with the
    // schedule runner. Don't expect a reply for broadcast frames.
    cli();
    _requestFrame = &frame;

    if (frame.nad() != LIN::kNodeAddressBroadcast) {
        _responseFrame = &frame;
    }

    sei();

    // wait for 2 cycles total (fatal if not completed by then)
    Timestamp t;

    while (!t.is_older_than(_scheduleLength * _frameTime * 2U)) {
        wdt_reset();

        if ((_requestFrame == nullptr) && (_responseFrame == nullptr)) {
            break;
        }
    }

    _requestFrame = nullptr;
    _responseFrame = nullptr;
}

void
Master::master_task(void *arg)
{
    auto *master = reinterpret_cast<Master *>(arg);

    master->_master_task();
}

void
Master::_master_task()
{
    // If the network is not awake, make sure the transceiver is turned off
    // and don't transmit anything.
    if (!_mtAwake) {
        return;
    }

    do {

        if (_mtIndex >= _scheduleLength) {
            // we hit the end of the pattern
            _mtIndex = 0;

            // safe point in the schedule to enable sleep, if requested and allowed
            if (_mtSleepRequest && !_testerPresent) {
                _mtAwake = false;
                // turn off the transceiver
                Board::lin_CS(false);
                return;
            }

#ifdef pinDebugStrobe
            // toggle the trigger signal for the first frame in each cycle
            pinDebugStrobe.toggle();
#endif

        }

        // get the next frame from the schedule
        LIN::FrameID fid = schedule_entry(_mtIndex++);

        // special handling for optional frame types
        switch (fid) {

        case LIN::kFrameIDConfigRequest:
        case LIN::kFrameIDConfigResponse:

            // only send in programmer mode
            if (!_testerPresent) {
                continue;
            }

            break;

        case LIN::kFrameIDMasterRequest:

            // skip unless we have a request frame
            if (_requestFrame == nullptr) {

                // if we still have a response frame, it's because we didn't
                // get a response when we called for it earlier; kill it now
                if (_responseFrame != nullptr) {
                    _responseFrame = nullptr;
                }

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

        // and transmit the header (will turn on the transceiver if required)
        mt_send_header(fid);

        break;
    } while (1);
}

void
Master::st_header_received()
{
    switch (current_FrameID()) {
    case LIN::kFrameIDRelays:
        // send the relay state
        st_send_response(relayFrame, 8);
        break;

    case LIN::kFrameIDConfigRequest:
        // collect any response from the programmer
        st_expect_response(8);
        break;

    case LIN::kFrameIDConfigResponse:
        // we may want to send the config response ourselves
        handle_config_response();
        break;

    case LIN::kFrameIDMasterRequest:

        // send the request if we have one
        if (_requestFrame != nullptr) {
            st_send_response(*_requestFrame, 8);
            _requestFrame = nullptr;
        }

        break;

    case LIN::kFrameIDSlaveResponse:
        // arrange to receive the response
        st_expect_response(8);
        break;

    default:
        LINDev::st_header_received();
        break;
    }
}

void
Master::st_response_received(LIN::Frame &frame)
{
    switch (current_FrameID()) {

    case LIN::kFrameIDConfigRequest:
        // may be a request for us
        handle_config_request(reinterpret_cast<LIN::ConfigFrame &>(frame));
        break;

    case LIN::kFrameIDSlaveResponse:

        // if we are expecting a response, copy it back
        if ((_requestFrame == nullptr) && (_responseFrame != nullptr)) {
            _responseFrame->copy(frame);
            _responseFrame = nullptr;
        }

        break;

    default:
        LINDev::st_response_received(frame);
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

    st_send_response(f, 8);
}
