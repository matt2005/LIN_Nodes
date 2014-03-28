///@master.h

#pragma once

#include <stdint.h>

#include "lin_protocol.h"
#include "timer.h"

class Event
{
public:
    Event(uint8_t slot, LIN::FID fid);

    static void     nextEvent();

private:
    static const uint8_t kMaxSlots = 2;

    static Event    *_slotHead[kMaxSlots];
    static Event    *_slotNext[kMaxSlots];
    static uint8_t  _currentSlot;

    Event           *_next;
    LIN::FID        _fid;
};


class Master
{
public:
    Master();

private:
    Timer           _timer;
};
