
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
    LIN::kFrameIDProxyRequest,  // skipped if no tester
    LIN::kFrameIDMasterRequest, // skipped if no work
    LIN::kFrameIDSlaveResponse, // skipped if no work
};

const uint8_t Master::_scheduleLength = sizeof(Master::_schedule) / sizeof(Master::_schedule[0]);

Master::Master() :
    Slave(LIN::kNodeAddressMaster),
    _mtTimer((Timer::Callback)Master::master_task, this, _frameTime),
    _mtIndex(0),
    _mtAwake(true),
    _mtSleepRequest(false),
    _requestFrame(nullptr),
    _responseFrame(nullptr),
    _testerPresent(false),
    _haveProxyRequest(false),
    _haveProxyResponse(false)
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
    // schedule runner. Don't expect a reply for broadcast frames (that's
    // what the functional NAD appears to be for).
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
    reinterpret_cast<Master *>(arg)->_master_task();
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

        case LIN::kFrameIDProxyRequest:

            // only send in programmer mode, don't send if we are in the process of
            // handling one
            if (!_testerPresent || _haveProxyRequest) {
                continue;
            }

            break;

        case LIN::kFrameIDMasterRequest:

            // skip unless we have a request frame or a proxy frame
            if ((_requestFrame == nullptr) && !_haveProxyRequest) {

                // if we still have a response frame, it's because we didn't
                // get a response when we called for it earlier; kill it now
                // so that the waiter can wake up more quickly
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
        st_send_response(relayFrame);
        break;

    case LIN::kFrameIDProxyRequest:
        // collect any response from the programmer
        st_expect_response();
        break;

    case LIN::kFrameIDMasterRequest:

        // send the explicit request if we have one
        if (_requestFrame != nullptr) {
            st_send_response(*_requestFrame);
            _requestFrame = nullptr;

            // send the proxied request if we have one

        } else if (_haveProxyRequest) {
            st_send_response(_proxyFrame);

            // if the proxy request was for us, we
            // need to prepare a reply
            if (master_request(_proxyFrame)) {
                _haveProxyResponse = true;
            }

            _haveProxyRequest = false;
        }

        break;

    case LIN::kFrameIDSlaveResponse:

        // if we have a proxy response, we should send it
        if (_haveProxyResponse) {
            st_send_response(_proxyFrame);
            _haveProxyResponse = false;

        } else if (_responseFrame != nullptr) {
            // arrange to receive the response for our own use
            st_expect_response();
        }

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

    case LIN::kFrameIDProxyRequest:
        // save for re-transmission
        _proxyFrame.copy(frame);
        _haveProxyRequest = true;
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
Master::sleep_requested(SleepType type)
{
    // ignore this
}

uint8_t
Master::get_param(uint8_t param)
{
    return masterParam(param);
}

void
Master::set_param(uint8_t param, uint8_t value)
{
    masterParam(param).set(value);
}
