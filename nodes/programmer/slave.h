///@file slave.h

#pragma once

#include "lin_slave.h"
#include "lin_protocol.h"

class ProgrammerSlave : public Slave
{
public:
    ProgrammerSlave() :
        Slave(LIN::kNodeAddressTester),
        _state(kStateIdle),
        _paramValue(0),
        _nodeAddress(0),
        _paramIndex(0),
        _suspended(false)
    {}

    void            reset()         { _state = kStateIdle; }

    bool            set_parameter(uint8_t nad, uint8_t param, uint8_t value);
    bool            get_parameter(uint8_t nad, uint8_t param, uint8_t &value);

    bool            is_idle()                const { return _state == kStateIdle; }
    bool            is_error()               const { return _state == kStateError; }

    void            setSuspend(bool suspended) { _suspended = suspended; }

protected:
    virtual void    header_received(LIN::FrameID fid) override;
    virtual void    response_received(LIN::FrameID fid, LIN::Frame &frame) override;
    virtual void    response_sent() override;
    virtual void    sleep_requested(SleepType type) override;
    virtual void    master_request(LIN::Frame &frame) override;

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
