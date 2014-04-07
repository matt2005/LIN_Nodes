///@master.h

#pragma once

#include <stdint.h>

#include "lin_protocol.h"
#include "lin_slave.h"
#include "timer.h"

class Event
{
public:
    Event(uint8_t slot, LIN::FID fid);

    static void     nextEvent();

    static uint16_t count;

private:
    static const uint8_t kMaxSlots = 2;

    static Event    *_slotHead[kMaxSlots];
    static Event    *_slotNext[kMaxSlots];
    static uint8_t  _currentSlot;

    Event           *_next;
    LIN::FID        _fid;
};


class Master : public Slave
{
public:
    Master();

    bool            doRequest(LIN::Frame &frame);
    bool            doRequestResponse(LIN::Frame &frame);

protected:
    virtual void headerReceived(LIN::FID fid) override;
    virtual void responseReceived(LIN::FID fid, LIN::Frame &frame) override;
    virtual void sleepRequested();

private:
    Timer           _timer;
};
