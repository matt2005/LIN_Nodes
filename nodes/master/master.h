/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

///@master.h

#pragma once

#include <stdint.h>

#include "lin_defs.h"
#include "lin_slave.h"
#include "timer.h"

class MasterNode : public Slave
{
public:
    MasterNode();

    Response relayFrame;

    /// Enable / disable sleep
    ///
    /// @param enable           If true, sleep is enabled. The master will enter
    ///                         sleep mode at the end of the current cycle
    ///                         if it is not in config mode.
    ///                         If false, sleep is inhibited.
    ///
    void            set_sleep_enable(bool enable)
    {
        _mtSleepRequest = enable;

        if (!enable) {
            _mtAwake = true;
        }
    }

protected:
    virtual void    st_header_received() override;
    virtual void    st_response_received(Response &frame) override;
    virtual void    st_sleep_requested(SleepType type) override;
    virtual bool    st_master_request(Response &frame) override;
    virtual bool    st_read_data(Parameter::Address address, uint16_t &value) override;
    virtual bool    st_write_data(Parameter::Address address, uint16_t value) override;

private:
    static const uint8_t   _schedule[];
    static const uint8_t        _scheduleLength;
    static const uint8_t        _frameTime = 10U;   //< milliseconds

    uint8_t         _testerPresent;         //< nonzero when a tester is present

    // master task state
    Timer           _mtTimer;
    uint8_t         _mtIndex;
    bool            _mtAwake;
    bool            _mtSleepRequest;

    /// master task
    static void     master_task(void *arg);
    void            _master_task();

    // slave task state
    Response        _stProxyFrame;

    bool            _stProxyRequest;      //< true when _proxyFrame needs to be sent as a Master Request
    bool            _stProxyResponse;     //< true when _proxyFrame needs to be sent as a Slave Response
    bool            _stExpectResponse;

    static uint8_t schedule_entry(uint8_t idx)
    {
        return (uint8_t)pgm_read_byte(&_schedule[idx]);
    }
};

extern MasterNode gMaster;
