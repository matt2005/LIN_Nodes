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
        _nodeAddress(0),
        _dataPage(0),
        _dataIndex(0),
        _dataValue(0),
        _suspended(false)
    {}

    void            reset()         { _state = kStateIdle; }

    bool            set_parameter(uint8_t nad, uint8_t param, uint8_t value);
    bool            get_parameter(uint8_t nad, uint8_t param, uint8_t &value);
    bool            get_error_count(uint8_t nad, uint8_t err, uint16_t &count);

    void            set_suspend(bool suspended) { _suspended = suspended; }

protected:
    virtual void    st_header_received() override;
    virtual void    st_response_received(LIN::Frame &frame) override;
    virtual void    st_sleep_requested(SleepType type) override;
    virtual bool    st_master_request(LIN::Frame &frame) override;

private:
    enum State : uint8_t {
        kStateIdle,
        kStateError,

        kStateSetData,
        kStateGetData,
        kStateWaitData
    };

    volatile State      _state;

    uint8_t             _nodeAddress;
    uint8_t             _dataPage;
    uint8_t             _dataIndex;
    volatile uint16_t    _dataValue;

    bool                _suspended;

    bool                wait_complete();
    bool                get_data_by_id(uint8_t nad, uint8_t page, uint8_t index, uint16_t &value);
    bool                set_data_by_id(uint8_t nad, uint8_t page, uint8_t index, uint16_t value);
};

extern ProgrammerSlave gSlave;
