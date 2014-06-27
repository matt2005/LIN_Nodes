///@file slave.h

#pragma once

#include "lin_slave.h"
#include "lin_protocol.h"

class ProgrammerSlave : public Slave
{
public:
    ProgrammerSlave() : 
        Slave(LIN::kNADProgrammer),
        _state(kStateIdle),
        _paramValue(0),
        _nodeAddress(0),
        _paramIndex(0),
        _suspended(false)
    {}

    void            reset()         { _state = kStateIdle; }

    bool            setParameter(uint8_t nad, uint8_t param, uint8_t value);
    bool            getParameter(uint8_t nad, uint8_t param, uint8_t &value);

    bool            isIdle()                const { return _state == kStateIdle; }
    bool            isError()               const { return _state == kStateError; }

    void            setSuspend(bool suspended) { _suspended = suspended; }

protected:
    virtual void    headerReceived(LIN::FID fid) override;
    virtual void    responseReceived(LIN::FID fid, LIN::Frame &frame) override;
    virtual void    responseSent() override;
    virtual void    sleepRequested(SleepType type);

private:
    enum State : uint8_t {
        kStateIdle,
        kStateError,

        kStateSetWaitRequest,
        kStateSetWaitSent,

        kStateGetWaitRequest,
        kStateGetWaitResponse,
        kStateGetComplete,
    };

    volatile State      _state;
    volatile uint8_t    _paramValue;

    uint8_t             _nodeAddress;
    uint8_t             _paramIndex;

    bool                _suspended;
};

extern ProgrammerSlave gSlave;
