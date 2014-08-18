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

#include "board.h"
#include "timer.h"

#include "slave.h"

ToolSlave::ToolSlave() :
    Slave(Tester::kNodeAddress, true)
{
}

void
ToolSlave::tick()
{
    switch (_masterState) {
    case kMSDisabled:
        // master is disabled, do nothing
        break;

    case kMSWaiting:

        // if the bus is idle, become the master
        if (!is_awake()) {
            _masterState = kMSRequest;
            _lastFrameStart.update();
        }

        break;

    case kMSRequest:
    case kMSResponse:

        // if we haven't been re-armed for a while, drop out of master mode
        if (_masterTimeout.is_older_than(1000U)) {
            _masterState = kMSDisabled;
            break;
        }

        // if we haven't reached the start of the next frame time, do nothing
        if (!_lastFrameStart.is_older_than(10)) {
            break;
        }

        // remember when we sent the next header
        _lastFrameStart.update();

        if (_masterState == kMSRequest) {
            // XXX optimisation - defer this until we send a response that
            // needs a SlaveResponse
            _masterState = kMSResponse;
            mt_send_header(kFrameIDMasterRequest);

        } else {
            _masterState = kMSRequest;
            mt_send_header(kFrameIDSlaveResponse);
        }

        break;
    }

    Slave::tick();
}

void
ToolSlave::set_data_by_id(uint8_t nad, Parameter::Address address, uint16_t value)
{
    _nodeAddress = nad;
    _dataAddress = address;
    _dataValue = value;
    _state = kStateSetData;
}

void
ToolSlave::get_data_by_id(uint8_t nad, Parameter::Address address)
{
    _nodeAddress = nad;
    _dataAddress = address;
    _dataValue = 0;
    _state = kStateGetData;
}

void
ToolSlave::send_bulk(uint8_t nad, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
{
    _nodeAddress = nad;
    _dataBytes[0] = d0;
    _dataBytes[1] = d1;
    _dataBytes[2] = d2;
    _dataBytes[3] = d3;
    _state = kStateBulkData;
}

void
ToolSlave::enable_master(bool enable)
{
    if (enable) {
        if (_masterState == kMSDisabled) {
            _masterState = kMSWaiting;
        }

        _masterTimeout.update();

    } else {
        _masterState = kMSDisabled;
    }
}

void
ToolSlave::st_header_received()
{
    _history.sawFID(current_FrameID());

    // we always want to see the response, for logging purposes
    st_expect_response();

    // do slave processing if we aren't the master
    if (_masterState <= kMSWaiting) {
        Slave::st_header_received();
    }

    // if we are the active master, we can respond to our own
    // requests
    if (_masterState > kMSWaiting) {
        Response resp;

        switch (current_FrameID()) {
        case kFrameIDMasterRequest:
            switch (_state) {
            case kStateSetData:
                resp.DataByID.nad = _nodeAddress;
                resp.DataByID.pci = pci::kSingleFrame;
                resp.DataByID.length = 5;
                resp.DataByID.sid = service_id::kWriteDataByID;
                resp.DataByID.index = _dataAddress;
                resp.DataByID.value = _dataValue;
                st_send_response(resp);
                _state = kStateIdle;
                break;

            case kStateBulkData:
                resp.MasterRequest.nad = _nodeAddress;
                resp.MasterRequest.pci = pci::kSingleFrame;
                resp.MasterRequest.length = 5;
                resp.MasterRequest.sid = service_id::kDataDump;
                resp.MasterRequest.d1 = _dataBytes[0];
                resp.MasterRequest.d2 = _dataBytes[1];
                resp.MasterRequest.d3 = _dataBytes[2];
                resp.MasterRequest.d4 = _dataBytes[3];
                st_send_response(resp);
                _state = kStateIdle;
                break;

            case kStateGetData:
                resp.DataByID.nad = _nodeAddress;
                resp.DataByID.pci = pci::kSingleFrame;
                resp.DataByID.length = 3;
                resp.DataByID.sid = service_id::kReadDataByID;
                resp.DataByID.index = _dataAddress;
                st_send_response(resp);
                _state = kStateWaitData;
                break;

            case kStateWaitData:
                // we have missed (or it never arrived) the reply to
                // the ReadDataByID operation
                _state = kStateError;
                break;

            default:
                break;
            }

            break;

        default:
            break;
        }
    }
}

void
ToolSlave::st_response_received(Response &resp)
{
    // save response for logging purposes
    _history.sawResponse(resp);

    switch (current_FrameID()) {
    case kFrameIDSlaveResponse:

        // is this a response to a current request?
        if ((_state == kStateWaitData) &&
            (resp.SlaveResponse.nad == _nodeAddress) &&
            (resp.SlaveResponse.sid == (service_id::kReadDataByID | service_id::kResponseOffset))) {

            // sanity-check the response
            if ((resp.DataByID.length != 5) ||
                (resp.DataByID.index != _dataAddress)) {
                _state = kStateError;

            } else {
                _dataValue = resp.DataByID.value;
                _state = kStateIdle;
            }
        }

        break;

    default:

        // do slave processing if we aren't the master
        if (_masterState <= kMSWaiting) {
            Slave::st_response_received(resp);
        }

        break;
    }
}

void
ToolSlave::st_sleep_requested(SleepType type)
{
    // XXX never sleep
}

bool
ToolSlave::st_master_request(Response &resp)
{
    bool reply = false;

    switch (resp.MasterRequest.sid) {
    case service_id::kTesterPresent:

        // tell the master that we're here, so that it will go away
        if ((_masterState == kMSWaiting) &&
            (resp.MasterRequest.nad == Tester::kNodeAddress)) {
            resp.MasterRequest.sid |= service_id::kResponseOffset;
            reply = true;
        }

        break;

    default:
        // do nothing here - we don't respond like a normal slave
        break;
    }

    return reply;
}

void
SlaveHistory::sawFID(uint8_t fid)
{
    // do we have a saved FID for a frame with no response?
    if (_FIDValid) {
        if (!full()) {
            _entries[_nextIn].time = _fidTime;
            _entries[_nextIn].frame[0] = _savedFID;
            _nextIn = next(_nextIn);
        }
    }

    // save the FID for when we see the response, or the next FID if not
    _savedFID = fid;
    _fidTime = Timer::time_now();
    _FIDValid = true;
}

void
SlaveHistory::sawResponse(Response &resp)
{
    if (!full()) {
        _entries[_nextIn].time = _fidTime;
        _entries[_nextIn].frame[0] = _savedFID | RQ_HISTORY_RESPONSE_VALID;
        _FIDValid = false;

        for (uint8_t i = 0; i < 8; i++) {
            _entries[_nextIn].frame[i + 1] = resp._bytes[i];
        }

        _nextIn = next(_nextIn);
    }
}

RQHistory *
SlaveHistory::get()
{
    if (empty()) {
        return nullptr;
    }

    RQHistory *result = &_entries[_nextOut];
    _nextOut = next(_nextOut);
    return result;
}

