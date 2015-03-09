/*
 * Copyright (c) 2012-2015, Mike Smith, <msmith@purgatory.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * o Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
    static const uint8_t   _scheduleLength;
    static const uint8_t   _frameTime = 10U;   //< milliseconds

    bool            _testerPresent;         //< true when there is a tester owning the bus

    // master task state
    Timer           _mtTimer;
    uint8_t         _mtIndex;
    bool            _mtAwake;
    bool            _mtSleepRequest;

    /// master task
    static void     master_task(void *arg);
    void            _master_task();

    bool            _stExpectResponse;

    static uint8_t schedule_entry(uint8_t idx)
    {
        return (uint8_t)pgm_read_byte(&_schedule[idx]);
    }
};

extern MasterNode gMaster;
