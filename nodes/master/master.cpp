/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */


#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "lin_drv.h"

#include "board.h"
#include "master.h"

// Schedule used in normal operation
//
PROGMEM const uint8_t MasterNode::_schedule[] = {
    kFrameIDRelays,
    kFrameIDProxyRequest,
    kFrameIDMasterRequest,
    kFrameIDSlaveResponse,
};

const uint8_t MasterNode::_scheduleLength = sizeof(MasterNode::_schedule) / sizeof(MasterNode::_schedule[0]);

MasterNode::MasterNode() :
    Slave(Master::kNodeAddress),
    _testerPresent(0),
    _mtTimer((Timer::Callback)MasterNode::master_task, this, _frameTime),
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
MasterNode::master_task(void *arg)
{
    reinterpret_cast<MasterNode *>(arg)->_master_task();
}

void
MasterNode::_master_task()
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
        uint8_t fid = schedule_entry(_mtIndex++);

        // special handling for optional frame types
        switch (fid) {

        case kFrameIDProxyRequest:

            if (_testerPresent == 0) {
                continue;
            }

            break;

        case kFrameIDMasterRequest:

            // XXX optimise - only send when we have something to send

            break;

        case kFrameIDSlaveResponse:

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
MasterNode::st_header_received()
{
    switch (current_FrameID()) {
    case kFrameIDRelays:
        // send the relay state
        st_send_response(relayFrame);
        break;

    case kFrameIDProxyRequest:
        // collect any response from the programmer
        st_expect_response();
        break;

    case kFrameIDMasterRequest:

        // if we have promised to proxy a request, send it immediately
        if (_stProxyRequest) {
            st_send_response(_stProxyFrame);

            // if the proxy request was for us, we
            // need to prepare a reply
            if ((_stProxyFrame.MasterRequest.nad == _nad) &&
                (st_master_request(_stProxyFrame))) {
                _stProxyResponse = true;
            }

            _stProxyRequest = false;

        } else {
            // XXX should be more flexible about what we send here...
            Response resp;

            resp.SlaveResponse.nad = Tester::kNodeAddress;
            resp.SlaveResponse.length = 2;
            resp.SlaveResponse.sid = service_id::kTesterPresent;
            resp.SlaveResponse.d1 = 0;
            resp.SlaveResponse.d2 = _testerPresent;
            st_send_response(resp);

            if (_testerPresent > 0) {
                _testerPresent--;
            }
        }

        _stExpectResponse = true;
        break;

    case kFrameIDSlaveResponse:

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

bool
MasterNode::st_master_request(Response &resp)
{
    bool reply = false;

    // ReadByID
    switch (resp.MasterRequest.sid) {

    case service_id::kReadByID:
        switch (resp.MasterRequest.d1) {
        case 0: // LIN product identification
            resp.ReadByID.length = 6;
            resp.ReadByID.sid |= service_id::kResponseOffset;
            resp.ReadByID.vendor = Master::kNodeSupplier;
            resp.ReadByID.function = Master::kNodeFunction;
            resp.ReadByID.variant = Master::kNodeVariant;
            reply = true;
            break;    
        }
    }

    if (reply == false) {
        reply = Slave::st_master_request(resp);
    }
    return reply;
}

void
MasterNode::st_response_received(Response &resp)
{
    switch (current_FrameID()) {

    case kFrameIDProxyRequest:
        // save for re-transmission
        _stProxyFrame = resp;
        _stProxyRequest = true;
        break;

    case kFrameIDSlaveResponse:

        // is this a response from a tester?
        if ((resp.SlaveResponse.nad == Tester::kNodeAddress) &&
            (resp.SlaveResponse.sid == (service_id::kTesterPresent | service_id::kResponseOffset))) {
            _testerPresent = 0xff;
        }

        break;

    default:
        // intentional bypass of Slave::st_response_received
        LINDev::st_response_received(resp);
        break;
    }
}

void
MasterNode::st_sleep_requested(SleepType type)
{
    // ignore this
}

bool
MasterNode::st_read_data(Parameter::Address address, uint16_t &value)
{
    // Handle node parameters
    uint8_t encoding = Master::param_encoding(address);
    if (encoding != kEncoding_none) {
        value = Parameter(address).get();
        return true;
    }

    // pass to superclass
    return Slave::st_read_data(address, value);
}

bool
MasterNode::st_write_data(Parameter::Address address, uint16_t value)
{
    uint8_t encoding = Master::param_encoding(address);
    if ((encoding != kEncoding_none) && !Encoding::invalid(encoding, value)) {
        Parameter(address).set(value);
        return true;
    }

    return Slave::st_write_data(address, value);
}
