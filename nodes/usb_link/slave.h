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

#include <timer.h>
#include <lin_slave.h>

#include "requests.h"

class SlaveHistory
{
public:

    SlaveHistory() :
        _savedFID(0),
        _FIDValid(false),
        _nextIn(0),
        _nextOut(0)
    {
    }

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
    static const uint8_t    _size = 8;
    RQHistory               _entries[_size + 1];

    uint8_t                 _savedFID;
    uint16_t                _fidTime;
    bool                    _FIDValid;
    uint8_t                 _nextIn;
    volatile uint8_t        _nextOut;

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

    bool            is_data_ready() const { return _state == kStateIdle; }
    bool            is_data_error() const { return _state == kStateError; }
    bool            is_waiting() const { return _masterState == kMSWaiting; }
    bool            is_master() const { return _masterState > kMSWaiting; }

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
        kStateBulkData,
        kStateWaitData
    };

    SlaveHistory        _history;
    State               _state = kStateIdle;

    uint8_t             _nodeAddress = 0;
    Parameter::Address  _dataAddress = 0;
    uint16_t            _dataValue = 0;
    uint8_t             _dataBytes[4];

    enum MasterState : uint8_t {
        kMSDisabled,            // master mode disabled
        kMSWaiting,             // waiting for the master to notice us and go offline
        kMSRequest,             // send master request next
        kMSResponse             // send slave response next
    };

    MasterState         _masterState = kMSDisabled;
    Timestamp           _lastFrameStart;
    Timestamp           _masterTimeout;
};
