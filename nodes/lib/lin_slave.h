///@file lin_slave.h
///
/// LIN driver wrapper for slaves
///

#pragma once

#include "lin_dev.h"
#include "lin_protocol.h"
#include "timer.h"

class Slave : public LINDev
{
public:
    Slave(LIN::NodeAddress nad, bool polled = kLINDevInterrupts);

    virtual void    tick() override;

protected:

    enum SleepType {
        kSleepTypeRequested,
        kSleepTypeIdle
    };
    static const uint16_t kIdleTimeout = 4000;  //< 4 seconds

    LIN::NodeAddress _nad;               //< node address 

    virtual void    st_header_received() override;
    virtual void    st_response_received(LIN::Frame &frame) override;

    /// Called when the network is told to sleep.
    ///
    /// @param type             Indicates whether this is a requested or idle
    ///                         timeout sleep request.
    ///
    virtual void    sleep_requested(SleepType type);

    /// Called when a LIN::kMasterRequest response is received that addresses
    /// this node.
    ///
    /// @param frame            The response frame
    /// @return                 If true, the (modified) frame should be sent
    ///                         as a slave response.
    ///
    virtual bool    master_request(LIN::Frame &frame);

    /// Prepare a response to the LIN::kSlaveResponse message.
    ///
    /// @param frame            The frame to send in response.
    ///
    void            slave_response(const LIN::Frame &frame) 
    {
        _response.copy(frame);
        _sendSlaveResponse = true;
    }

    /// Get a parameter
    ///
    /// Called when handling kServiceIDReadDataByID
    ///
    /// @param param            Parameter index to be read.
    /// @return                 The parameter's current value.
    ///
    virtual uint8_t         get_param(uint8_t param);

    /// Set a parameter
    ///
    /// Called when handling kServiceIDWriteDataByID
    ///
    /// @param param            Parameter index to be written
    /// @param value            Value to be written.
    ///
    virtual void            set_param(uint8_t param, uint8_t value);

private:
    Timer           _idleTimer;         //< Bus idle timer
    LIN::Frame      _response;          //< canned response frame

    bool            _sendSlaveResponse:1;

    static void     idle_timeout(void *arg); //< idle timeout callback
};
