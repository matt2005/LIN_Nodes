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

#include "requests.h"
#include "lin_slave.h"
#include "lin_protocol.h"

class SlaveHistory
{
public:

    SlaveHistory() : _nextIn(0), _nextOut(0) {}

    /// Save a received FID into the nextIn entry regardless of whether
    /// we are ready to push the remainder of the frame just yet.
    ///
    /// @param fid              A just-received FID
    ///
    void            pushFID(uint8_t fid)
    {
        // Have we already pushed a FID, but not a response? If so, there
        // was no response in the frame, so we should push an empty entry if
        // we can.
        //
        if (_entries[_nextIn].bytes[0] & RQ_HISTORY_FRAME_VALID) {
            if (full()) {
                // overflow, try to mark this
                _entries[_nextIn].bytes[0] = 0xbf;
            } else {
                // advance without a response in the frame
                _nextIn = next(_nextIn);            
            }
        }

        // Set the FID in the next-in frame regardless of whether we
        // can actually use it yet...
        fid = (fid | RQ_HISTORY_FRAME_VALID) & ~RQ_HISTORY_RESPONSE_VALID;
        _entries[_nextIn].bytes[0] = fid;
    }

    /// Push a just-received or just-about-to-be-sent response (plus the
    /// previously-saved FID) into the ring if there is space for it.
    ///
    /// @param f                The response to push.
    /// 
    void            pushResponse(LIN::Frame &f)
    {
        if (full()) {
            // ring overflow, set a magic FID to help us debug
            _entries[_nextIn].bytes[0] = 0xff;            
        } else {
            // copy the response and advance the ring
            for (uint8_t i = 0; i < 8; i++) {
                _entries[_nextIn].bytes[i + 1] = f[i];
            }
            _entries[_nextIn].bytes[0] |= RQ_HISTORY_RESPONSE_VALID;
            _nextIn = next(_nextIn);
        }
    }

    /// Pull a frame out of the queue
    ///
    /// @param buf              destination buffer
    /// @return                 true if a frame was pulled
    ///
    bool            pull(uint8_t buf[9])
    {
        if (empty()) {
            return false;
        }
        for (uint8_t i = 0; i < 9; i++) {
            buf[i] = _entries[_nextOut].bytes[i];
        }
        _entries[_nextOut].bytes[0] = 0;
        _nextOut = next(_nextOut);
        return true;
    }

private:
    struct Entry
    {
        uint8_t     bytes[9];
    };

    static const uint8_t    _size = 8;
    volatile Entry          _entries[_size + 1];

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

    bool            get_history(uint8_t buf[9]) { return _history.pull(buf); }

    bool            get_data_by_id(uint8_t nad, uint8_t page, uint8_t index, uint16_t &value);
    bool            set_data_by_id(uint8_t nad, uint8_t page, uint8_t index, uint16_t value);

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

    SlaveHistory        _history;
    volatile State      _state;

    uint8_t             _nodeAddress;
    uint8_t             _dataPage;
    uint8_t             _dataIndex;
    volatile uint16_t   _dataValue;

    bool                wait_complete();
};
