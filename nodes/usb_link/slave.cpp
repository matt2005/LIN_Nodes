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

    // master schedule
    switch (_masterState) {
    case kMSDisabled:
        break;
    case kMSWaiting:
        if (is_awake()) {
            // still waiting for the bus to go idle
            break;
        }
        _masterState = kMSRequest;
        _lastFrameStart.update();
        break;
    case kMSRequest:
        _masterState = kMSResponse;
        _lastFrameStart.update();
        mt_send_header(kFrameIDMasterRequest);
        break;
    case kMSResponse:
        _masterState = kMSRequest;
        _lastFrameStart.update();
        mt_send_header(kFrameIDMasterRequest);
        break;
    }

    Slave::tick();
}

void
ToolSlave::set_data_by_id(uint8_t nad, Parameter::Address address, uint16_t value)
{
    if (_masterState > kMSWaiting) {
        _nodeAddress = nad;
        _dataAddress = address;
        _dataValue = value;
        _state = kStateSetData;
    }
}

void
ToolSlave::get_data_by_id(uint8_t nad, Parameter::Address address)
{
    if (_masterState >= kMSWaiting) {
        _nodeAddress = nad;
        _dataAddress = address;
        _dataValue = 0;
        _state = kStateGetData;
    }
}

void
ToolSlave::send_bulk(uint8_t nad, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
{
    if (_masterState >= kMSWaiting) {
        _nodeAddress = nad;
        _dataBytes[0] = d0;
        _dataBytes[1] = d1;
        _dataBytes[2] = d2;
        _dataBytes[3] = d3;
        _state = kStateBulkData;
    }
}

void
ToolSlave::enable_master(bool enable)
{
    if (enable) {
        if (is_awake()) {
            _masterState = kMSWaiting;
        } else {
            _masterState = kMSRequest;
            _lastFrameStart.update();
        }
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

    Response resp;

    switch (current_FrameID()) {
    case kFrameIDMasterRequest:
        switch (_state) {
        case kStateSetData:
            resp.DataByID.nad = _nodeAddress;
            resp.DataByID.length = 5;
            resp.DataByID.sid = service_id::kWriteDataByID;
            resp.DataByID.index = _dataAddress;
            resp.DataByID.value = _dataValue;
            st_send_response(resp);
            _state = kStateIdle;
            break;

        case kStateBulkData:
            resp.MasterRequest.nad = _nodeAddress;
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
        // don't pass the master request header to the slave code,
        // as it will cancel the responses we just set up.
        break;

    default:
        Slave::st_header_received();
        break;
    }
}

void
ToolSlave::st_response_received(Response &resp)
{
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
        break;
    }

    Slave::st_response_received(resp);
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
        // If we want the master go offline, tell it we're present
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
            _entries[_nextIn].bytes[0] = _savedFID;
            _nextIn = next(_nextIn);
        }
    }

    _savedFID = fid;
    _FIDValid = true;
}

void
SlaveHistory::sawResponse(Response &resp)
{
    if (!full()) {
        _entries[_nextIn].bytes[0] = _savedFID | RQ_HISTORY_RESPONSE_VALID;
        _FIDValid = false;

        for (uint8_t i = 0; i < 8; i++) {
            _entries[_nextIn].bytes[i + 1] = resp._bytes[i];
        }

        _nextIn = next(_nextIn);
    }
}

uint8_t *
SlaveHistory::get()
{
    if (empty()) {
        return nullptr;
    }

    uint8_t *result = &_entries[_nextOut].bytes[0];
    _nextOut = next(_nextOut);
    return result;
}

