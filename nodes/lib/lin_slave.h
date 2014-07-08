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

    /// Prepare a response to the LIN::kSlaveResponse message.
    ///
    /// @param frame            The frame to send in response.
    ///
    void            slaveResponse(const LIN::Frame &frame) 
    {
        _response.copy(frame);
        _sendSlaveResponse = true;
    }

    /// Get a parameter
    ///
    /// Called when handling config requests
    ///
    /// @param param            Parameter index to be read.
    /// @return                 The parameter's current value.
    ///
    virtual uint8_t         get_param(uint8_t param);

    /// Set a parameter
    ///
    /// Called when handling config responses
    ///
    /// @param param            Parameter index to be written
    /// @param value            Value to be written.
    ///
    virtual void            set_param(uint8_t param, uint8_t value);

private:
    Timer           _idleTimer;         //< Bus idle timer
    LIN::Frame      _response;          //< canned response frame
    uint8_t         _configParam;       //< parameter to send for ConfigResponse

    bool            _sendSlaveResponse:1;
    bool            _sendConfigResponse:1;

    static void     idleTimeout(void *arg); //< idle timeout callback

    void            configRequest(LIN::ConfigFrame &frame);
    void            configResponse();
};
