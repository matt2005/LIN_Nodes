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
    Slave(LIN::NodeAddress nad);

protected:

    enum SleepType {
        kSleepRequested,
        kSleepIdle
    };
    static const uint16_t kIdleTimeout = 4000;  //< 4 seconds

    LIN::NodeAddress _nad;               //< node address 

    /// Prepare a response to the LIN::kSlaveResponse message.
    ///
    /// @param frame            The frame to send in response.
    ///
    void            prepareSlaveResponse(const LIN::Frame &frame) 
    {
        _slaveResponse.copy(frame);
    }

    /// Called when a header has been received.
    ///
    /// Must be implemented by the subclass.
    ///
    /// @param fid              The FID from the received header.
    ///
    virtual void    headerReceived(LIN::FID fid) override;
    
    /// Called when a response requested by requestResponse has been received.
    ///
    /// @param fid              The FID from the header associated with this
    ///                         response.
    /// @param frame            The received response frame.
    ///
    virtual void    responseReceived(LIN::FID fid, LIN::Frame &frame) override;
    
    /// Called when a response has been sent
    ///
    virtual void    responseSent() override;

    /// Called when the network is told to sleep.
    ///
    /// @param type             Indicates whether this is a requested or idle
    ///                         timeout sleep request.
    ///
    virtual void    sleepRequested(SleepType type);

    /// Called when a LIN::kMasterRequest response is received that addresses
    /// this node.
    ///
    /// @param frame            The response frame
    ///
    virtual void    masterRequest(LIN::Frame &frame);

private:
    Timer           _idleTimer;         //< Bus idle timer
    LIN::Frame      _slaveResponse;     //< slave response from previous master request

    static void     idleTimeout(void *arg); //< idle timeout callback
};
