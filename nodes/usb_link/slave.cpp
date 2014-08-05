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
    Slave(Tester::kNodeAddress, true),
    _state(kStateIdle),
    _nodeAddress(0),
    _dataPage(0),
    _dataIndex(0),
    _dataValue(0)
{
}

void
ToolSlave::set_data_by_id(uint8_t nad, uint8_t page, uint8_t index, uint16_t value)
{
    _nodeAddress = nad;
    _dataPage = page;
    _dataIndex = index;
    _dataValue = value;
    _state = kStateSetData;
}

void
ToolSlave::get_data_by_id(uint8_t nad, uint8_t page, uint8_t index)
{
    _nodeAddress = nad;
    _dataPage = page;
    _dataIndex = index;
    _dataValue = 0;
    _state = kStateGetData;
}

void
ToolSlave::st_header_received()
{
    _history.sawFID(current_FrameID());

    switch (current_FrameID()) {
    case kFrameIDProxyRequest:
        switch (_state) {
        case kStateSetData:
            st_send_response(Response(_nodeAddress,
                                        5,
                                        service_id::kWriteDataByID,
                                        _dataIndex,
                                        _dataPage,
                                        _dataValue & 0xff,
                                        _dataValue << 8));
            _state = kStateIdle;
            break;

        case kStateGetData:
            st_send_response(Response(_nodeAddress,
                                        3,
                                        service_id::kReadDataByID,
                                        _dataIndex,
                                        _dataPage));
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
        Slave::st_header_received();
        break;
    }

    // we always want to see the response, for logging purposes
    st_expect_response();
}

void
ToolSlave::st_response_received(Response &frame)
{
    _history.sawResponse(frame);

    switch (current_FrameID()) {
    case kFrameIDSlaveResponse:

        // is this a response to a current request?
        if ((_state == kStateWaitData) &&
            (Signal::nad(frame) == _nodeAddress) &&
            (Signal::sid(frame) == (service_id::kReadDataByID | service_id::kResponseOffset))) {

            // sanity-check the response
            if ((Signal::pci(frame) != 5) ||
                (Signal::d1(frame) != _dataIndex) ||
                (Signal::d2(frame) != _dataPage)) {
                _state = kStateError;

            } else {
                _dataValue = Signal::d3(frame) | (Signal::d4(frame) << 8);
                _state = kStateIdle;
                debugc('t');
            }
        }

        break;

    default:
        break;
    }

    Slave::st_response_received(frame);
}

void
ToolSlave::st_sleep_requested(SleepType type)
{
    // XXX never sleep
}

bool
ToolSlave::st_master_request(Response &frame)
{
    bool reply = false;

    switch (Signal::sid(frame)) {
    case service_id::kTesterPresent:

        // send a positive response to a directly-addressed request
        if ((Signal::nad(frame) == Tester::kNodeAddress)) {
            Signal::sid(frame).set(Signal::sid(frame) | service_id::kResponseOffset);
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
SlaveHistory::sawResponse(Response &f)
{
    if (!full()) {
        _entries[_nextIn].bytes[0] = _savedFID | RQ_HISTORY_RESPONSE_VALID;
        _FIDValid = false;

        for (uint8_t i = 0; i < 8; i++) {
            _entries[_nextIn].bytes[i + 1] = f[i];
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

