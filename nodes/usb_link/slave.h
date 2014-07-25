///@file slave.h

#pragma once

#include "lin_slave.h"
#include "lin_protocol.h"

class FrameRing
{
public:
    FrameRing() : _nextIn(0), _nextOut(0) {}

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
        if (!full() && _entries[_nextIn].fidValid) {
            _nextIn = next(_nextIn);            
        }
        _entries[_nextIn].fid = fid;
        _entries[_nextIn].responseValid = 0;
        _entries[_nextIn].fidValid = 1;
    }

    /// Push a just-received or just-about-to-be-sent response (plus the
    /// previously-saved FID) into the ring if there is space for it.
    ///
    /// @param f                The response to push.
    /// 
    void            pushResponse(LIN::Frame &f)
    {
        if (!full()) {
            _entries[_nextIn].response.copy(f);
            _entries[_nextIn].responseValid = 1;
            _nextIn = next(_nextIn);
        }
    }

    /// Pull a frame out of the queue
    ///
    /// @param fid              FID for the frame
    /// @param f                response for the frame
    /// @param responseValid    set if the response is valid
    /// @return                 true if a frame was pulled
    ///
    bool            pull(uint8_t &fid, uint8_t response[8], bool &responseValid)
    {
        if (empty()) {
            return false;
        }
        fid = _entries[_nextOut].fid;
        if (_entries[_nextOut].responseValid) {
            responseValid = true;
            response[0] = _entries[_nextOut].response[0];
            response[1] = _entries[_nextOut].response[1];
            response[2] = _entries[_nextOut].response[2];
            response[3] = _entries[_nextOut].response[3];
            response[4] = _entries[_nextOut].response[4];
            response[5] = _entries[_nextOut].response[5];
            response[6] = _entries[_nextOut].response[6];
            response[7] = _entries[_nextOut].response[7];
        } else {
            responseValid = false;
        }
        _nextOut = next(_nextOut);
        return true;
    }

private:
    struct Entry
    {
        uint8_t     spare:1;
        uint8_t     responseValid:1;
        uint8_t     fidValid:1;
        uint8_t     fid:6;
        LIN::Frame  response;
    };

    static const uint8_t    _size = 8;
    Entry                   _entries[_size + 1];

    uint8_t                 _nextIn;
    volatile uint8_t        _nextOut;

    uint8_t         next(uint8_t index) const { return (index < _size) ? index + 1 : 0; }
    bool            empty() const { return _nextIn == _nextOut; }
    bool            full() const { return next(_nextIn) == _nextOut; }
};

class ToolSlave : public Slave
{
public:
    ToolSlave() :
        Slave(LIN::kNodeAddressTool, true),
        _state(kStateIdle),
        _nodeAddress(0),
        _dataPage(0),
        _dataIndex(0),
        _dataValue(0)
    {}

    void            reset()         { _state = kStateIdle; }

    bool            get_data_by_id(uint8_t nad, uint8_t page, uint8_t index, uint16_t &value);
    bool            set_data_by_id(uint8_t nad, uint8_t page, uint8_t index, uint16_t value);

    bool            get_history(uint8_t &fid_flags, uint8_t response[8])
    {
        bool responseValid;
        if (_history.pull(fid_flags, response, responseValid)) {
            if (responseValid) {
                fid_flags |= (1<<7);
            }
            return true;
        }
        return false;
    }

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

    FrameRing           _history;
    volatile State      _state;

    uint8_t             _nodeAddress;
    uint8_t             _dataPage;
    uint8_t             _dataIndex;
    volatile uint16_t   _dataValue;

    bool                wait_complete();
};
