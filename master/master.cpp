
#include <avr/interrupt.h>

#include "lin_drv.h"

#include "board.h"
#include "master.h"

Event   *Event::_slotHead[kMaxSlots];
Event   *Event::_slotNext[kMaxSlots];
uint8_t Event::_currentSlot;
uint16_t Event::count;

Event::Event(uint8_t slot, LIN::FID fid) :
    _next(_slotHead[slot]),
    _fid(fid)
{
    _slotHead[slot] = this;
}

void
Event::nextEvent()
{
    count++;

    // Invoke the pending slot handler, if there is one.
    //
    // If there are no handlers for this slot, we will sit idle until the next one.
    //
    if (_slotNext[_currentSlot] == nullptr) {
        _slotNext[_currentSlot] = _slotHead[_currentSlot];
    }
    if (_slotNext[_currentSlot] != nullptr) {
        LIN::FID fid = _slotNext[_currentSlot]->_fid;

        // turn on the LIN driver
        Board::linCS(true);

        // and transmit the header
        lin_tx_header(LIN_2X, fid, 0);
    }
    _slotNext[_currentSlot] = _slotNext[_currentSlot]->_next;

    // move to the next slot, or back to the top
    if (++_currentSlot >= kMaxSlots) {
        _currentSlot = 0;
    }
}

Master::Master() :
    Slave(LIN::kNADMaster),
    _eventTimer((Timer::Callback)Event::nextEvent, (Timer::Period)10)
{
}

bool
Master::doRequest(LIN::Frame &frame)
{
    cli();
    _requestFrame = &frame;
    _responseFrame = nullptr;
    sei();

    return waitRequest();
}

bool
Master::doRequestResponse(LIN::Frame &frame)
{
    cli();
    _requestFrame = &frame;
    _responseFrame = &frame;
    sei();

    return waitRequest();    
}

bool
Master::waitRequest()
{
    // spin for 100ms waiting for the frame to be sent
    _requestTimer.setRemaining(100);
    while (!_requestTimer.didExpire()) {
        if ((_requestFrame == nullptr) &&
            (_responseFrame == nullptr)) {
            _requestTimer.setRemaining(0);
            return true;
        }
    }
    _requestFrame = nullptr;
    _responseFrame = nullptr;
    return false;
}

void
Master::headerReceived(LIN::FID fid)
{
    switch (fid) {
    case LIN::kFIDControls:
        break;

    case LIN::kFIDMasterRequest:
        // do slave processing first
        Slave::headerReceived(fid);

        // if we have a request to send, commit it to the wire
        if (_requestFrame != nullptr) {
            sendResponse(*_requestFrame, 8);
            _requestFrame = nullptr;
        }
        break;

    case LIN::kFIDSlaveResponse:
        // arrange to receive the response if a slave sends it
        requestResponse(8);

        // and do slave processing
        Slave::headerReceived(fid);
        break;

    default:
        Slave::headerReceived(fid);
        break;
    }
}

void
Master::responseReceived(LIN::FID fid, LIN::Frame &frame)
{
    switch (fid) {

    case LIN::kFIDSlaveResponse:
        // if we are expecting a response, copy it back
        if (_responseFrame != nullptr) {
            *_responseFrame = frame;
            _responseFrame = nullptr;
        }
        break;

    default:
        Slave::responseReceived(fid, frame);
        break;
    }
}

void
Master::sleepRequested()
{
    // XXX might be OK to sleep here, really...
}
