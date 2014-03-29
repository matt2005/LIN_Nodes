
#include "lin_drv.h"

#include "board.h"
#include "master.h"

Event   *Event::_slotHead[kMaxSlots];
Event   *Event::_slotNext[kMaxSlots];
uint8_t Event::_currentSlot;

Event::Event(uint8_t slot, LIN::FID fid) :
    _next(_slotHead[slot]),
    _fid(fid)
{
    _slotHead[slot] = this;
}

void
Event::nextEvent()
{
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
        pinLINCS.set();

        // special handling for the LIN master/slave requests
        if (fid < LIN::kFIDMasterRequest) {
            lin_tx_header(LIN_2X, fid, 0);
        } else {
            lin_tx_header(LIN_1X, fid, 8);
        }
    }
    _slotNext[_currentSlot] = _slotNext[_currentSlot]->_next;

    // move to the next slot, or back to the top
    if (++_currentSlot >= kMaxSlots) {
        _currentSlot = 0;
    }
}

Master::Master() :
    _timer(10, (Timer::Callback)Event::nextEvent)
{
}
