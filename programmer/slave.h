///@file slave.h

#pragma once

#include "lin_slave.h"
#include "lin_protocol.h"

class ProgrammerSlave : public Slave
{
public:
    ProgrammerSlave() : Slave(LIN::kNADProgrammer) {}

    void            setParameter(LIN::NodeAddress nad, uint8_t param, uint8_t value);
    void            requestParameter(LIN::NodeAddress nad, uint8_t param);
    uint8_t         getParameter() const { return _paramValue; }
    void            reset() { _state = kStateIdle; }
    bool            isError() const { return _state == kStateError; }
    bool            isParamAvailable() const { return _state == kStateGetComplete; }

protected:
    virtual void    headerReceived(LIN::FID fid) override;
    virtual void    responseReceived(LIN::FID fid, LIN::Frame &frame) override;
    virtual void    responseSent() override;

private:
    enum State : uint8_t {
        kStateIdle,

        kStateSetWaitRequest,
        kStateSetWaitSent,

        kStateGetWaitRequest,
        kStateGetWaitSlaveResponse,
        kStateGetWaitSlaveResponseData,
        kStateGetComplete,

        kStateError
    };

    volatile State      _state;
    uint8_t             _nodeAddress;
    volatile uint8_t    _paramIndex;
    volatile uint8_t    _paramValue;

};
