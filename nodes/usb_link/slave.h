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

///@file slave.h

#pragma once

#include <timer.h>
#include <lin_slave.h>

#include "requests.h"

class SlaveHistory
{
public:

    SlaveHistory() {}

    /// Save a received FID into the nextIn entry regardless of whether
    /// we are ready to push the remainder of the frame just yet.
    ///
    /// @param fid              A just-received FID
    ///
    void            sawFID(uint8_t fid);

    /// Push a just-received or just-about-to-be-sent response (plus the
    /// previously-saved FID) into the ring if there is space for it.
    ///
    /// @param f                The response to push.
    ///
    void            sawResponse(Response &f);

    /// Get a pointer to the oldest frame in the queue.
    /// This will remain valid until at least the next poll cycle.
    ///
    /// @return                 true if a frame was pulled
    ///
    RQHistory       *get();

private:
    static const uint8_t    _size = 16;
    RQHistory               _entries[_size + 1] = {};

    uint8_t                 _savedFID = 0;
    uint16_t                _fidTime = 0;
    bool                    _FIDValid = false;
    uint8_t                 _nextIn = 0;
    volatile uint8_t        _nextOut = 0;

    uint8_t         next(uint8_t index) const { return (index < _size) ? index + 1 : 0; }
    bool            empty() const { return _nextIn == _nextOut; }
    bool            full() const { return next(_nextIn) == _nextOut; }
};

class ToolSlave : public Slave
{
public:
    ToolSlave();

    virtual void    tick() override;

    RQHistory       *get_history() { return _history.get(); }

    void            get_data_by_id(uint8_t nad, Parameter::Address address);
    void            set_data_by_id(uint8_t nad, Parameter::Address address, uint16_t value);
    void            send_bulk(uint8_t nad, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);
    void            enable_master(bool enable);

    uint16_t        *get_data() { return &_dataValue; }

    bool            is_data_ready() const
    {
        return ((_state < kStateMaxIdle) && (is_awake() || is_master()));
    }
    bool            is_data_idle() const
    {
        return (_state < kStateMaxIdle);
    }
    bool            is_data_error() const { return _state == kStateError; }
    bool            is_data_rejected() const { return _state == kStateRejected; }
    bool            is_waiting() const { return _masterState == kMSWaiting; }
    bool            is_master() const { return _masterState > kMSWaiting; }

protected:
    virtual void    st_header_received() override;
    virtual void    st_response_received(Response &frame) override;
    virtual void    st_sleep_requested(SleepType type) override;
    virtual bool    st_master_request(Response &frame) override;

private:
    enum State : uint8_t {
        // idle states (ready for a new transaction)
        kStateIdle,
        kStateError,
        kStateRejected,
        kStateMaxIdle,

        // master-only states
        kStateSetData,
        kStateBulkData,

        // master/slave states
        kStateGetData,
        kStateWaitData
    };

    SlaveHistory        _history;
    State               _state = kStateIdle;

    uint8_t             _nodeAddress = 0;
    Parameter::Address  _dataAddress = 0;
    uint16_t            _dataValue = 0;
    uint8_t             _dataBytes[4] = {0};

    enum MasterState : uint8_t {
        kMSDisabled,            // master mode disabled
        kMSWaiting,             // waiting for the master to notice us and go offline
        kMSRequest,             // send master request next
        kMSResponse             // send slave response next
    };

    MasterState         _masterState = kMSDisabled;
    Timestamp           _lastFrameStart;
    Timestamp           _masterTimeout;

    void            refresh_master();
};
