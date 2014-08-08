/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

///@file slave.h

#pragma once

#include "lin_slave.h"

class ProgrammerSlave : public Slave
{
public:
    ProgrammerSlave() :
        Slave(Tester::kNodeAddress),
        _state(kStateIdle),
        _nodeAddress(0),
        _dataAddress(0),
        _dataValue(0),
        _suspended(false)
    {}

    void            reset()         { _state = kStateIdle; }

    bool            set_parameter(uint8_t nad, Parameter::Address Address, uint8_t value);
    bool            get_parameter(uint8_t nad, Parameter::Address Address, uint8_t &value);
    bool            get_error_count(uint8_t nad, uint8_t err, uint16_t &count);

    void            set_suspend(bool suspended) { _suspended = suspended; }

protected:
    virtual void    st_header_received() override;
    virtual void    st_response_received(Response &frame) override;
    virtual void    st_sleep_requested(SleepType type) override;
    virtual bool    st_master_request(Response &frame) override;

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
    Parameter::Address  _dataAddress;
    volatile uint16_t    _dataValue;

    bool                _suspended;

    bool                wait_complete();
    bool                get_data_by_id(uint8_t nad, Parameter::Address address, uint16_t &value);
    bool                set_data_by_id(uint8_t nad, Parameter::Address address, uint16_t value);
};

extern ProgrammerSlave gSlave;
