
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
    LIN::kFrameIDProxyRequest,
    LIN::kFrameIDMasterRequest,
    LIN::kFrameIDSlaveResponse,
};

const uint8_t Master::_scheduleLength = sizeof(Master::_schedule) / sizeof(Master::_schedule[0]);

Master::Master() :
    Slave(LIN::kNodeAddressMaster),
    _testerPresent(0),
    _mtTimer((Timer::Callback)Master::master_task, this, _frameTime),
    _mtIndex(0),
    _mtAwake(true),
    _mtSleepRequest(false),
    _stProxyRequest(false),
    _stProxyResponse(false),
    _stExpectResponse(false)
{
#ifdef pinDebugStrobe
    pinDebugStrobe.clear();
    pinDebugStrobe.cfg_output();
#endif
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

            if (_testerPresent == 0) {
                continue;
            }

            break;

        case LIN::kFrameIDMasterRequest:

            // XXX optimise - only send when we have something to send

            break;

        case LIN::kFrameIDSlaveResponse:

            // only send when something is expected to respond
            if (!_stProxyResponse && !_stExpectResponse) {
                continue;
            }

            _stExpectResponse = false;

            break;

        default:
            // send the frame as requested
            break;
        }

        // and transmit the header
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

        // if we have promised to proxy a request, send it immediately
        if (_stProxyRequest) {
            st_send_response(_stProxyFrame);

            // if the proxy request was for us, we
            // need to prepare a reply
            if ((_stProxyFrame.nad() == LIN::kNodeAddressMaster) &&
                (st_master_request(_stProxyFrame))) {
                _stProxyResponse = true;
            }

            _stProxyRequest = false;

        } else {
            // XXX should be more flexible about what we send here...
            st_send_response(LIN::Frame(LIN::kNodeAddressTester,
                                        2,
                                        LIN::kServiceIDTesterPresent,
                                        0,
                                        _testerPresent));

            if (_testerPresent > 0) {
                _testerPresent--;
            }
        }

        _stExpectResponse = true;
        break;

    case LIN::kFrameIDSlaveResponse:

        // if we have a proxy response, we should send it
        if (_stProxyResponse) {
            st_send_response(_stProxyFrame);
            _stProxyResponse = false;

        } else {
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
        _stProxyFrame.copy(frame);
        _stProxyRequest = true;
        break;

    case LIN::kFrameIDSlaveResponse:

        // is this a response from a tester?
        if ((frame.nad() == LIN::kNodeAddressTester) &&
            (frame.sid() == (LIN::kServiceIDTesterPresent | LIN::kServiceIDResponseOffset))) {
            _testerPresent = 0xff;
        }

        break;

    default:
        // intentional bypass of Slave::st_response_received
        LINDev::st_response_received(frame);
        break;
    }
}

void
Master::st_sleep_requested(SleepType type)
{
    // ignore this
}

bool
Master::st_read_data(uint8_t page, uint8_t index, uint16_t &value)
{
    bool result = false;

    switch (page) {
    case kDataPageStatus:
        // XXX should implement this in a somewhat-generic fashion?
        break;

    case kDataPageNodeParameters:
        if (index < kMasterParamMax) {
            value = masterParam(index);
            result = true;
        }

        break;

    }

    if (!result) {
        result = Slave::st_read_data(page, index, value);
    }

    return result;
}

bool
Master::st_write_data(uint8_t page, uint8_t index, uint16_t value)
{
    bool result = false;

    switch (page) {
    case kDataPageNodeParameters:
        if (index < kMasterParamMax) {
            masterParam(index).set(value & 0xff);
            result = true;
        }

        break;

    default:
        break;
    }

    if (!result) {
        result = Slave::st_write_data(page, index, value);
    }

    return result;
}
