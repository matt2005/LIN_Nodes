///@file lin_slave.h
///
/// LIN driver wrapper for slaves
///

#pragma once

#include "lin_drv.h"
#include "lin_protocol.h"
#include "timer.h"

class Slave
{
public:
    Slave(LIN::NodeAddress nad);

    /// Called from the transfer-complete ISR
    ///
    void            isrTC();
    
    /// Called from the error ISR
    ///
    void            isrError();

    // test mode
    void            masterTest();

    enum Error : uint8_t {
        kErrLine,                       //< readback error when transmitting
        kErrChecksum,                   //< received data checksum mismatch
        kErrParity,                     //< header parity error
        kErrFraming,                    //< framing error
        kErrSynchronisation,            //< bitrate synchronisation error
        kErrProtocol,                   //< slave protocol error
        kErrMax
    };

    uint16_t        errors[kErrMax];     //< error counters

protected:

    enum SleepType {
        kSleepRequested,
        kSleepIdle
    };
    static const uint16_t kIdleTimeout = 4000;  //< 4 seconds

    LIN::NodeAddress _nad;               //< node address 

    /// As the driver to pay attention to the next response that it sees.
    ///
    /// This will usually be the response associated with an interesting
    /// header.
    ///
    /// @param length           The length of the response to be received.
    ///
    void            requestResponse(uint8_t length);
    
    /// Send a LIN header. This is strictly speaking a master function, but
    /// it shares functionality with the slave.
    ///
    /// @param fid              The Frame ID to send in the header.
    ///
    void            sendHeader(LIN::FrameID fid);

    /// Send a response.
    ///
    /// Subclass implementation of headerReceived will call when it wants ti
    /// send a response for an interesting header.
    ///
    /// Responses are always sent in LIN 2.x format, unless the FID from the
    /// header implies 1.x (i.e. SlaveResponse).
    ///
    /// @param frame            The response frame to send.
    /// @param length           The length of the response frame.
    ///
    void            sendResponse(LIN::Frame &frame, uint8_t length);

    /// Prepare a response to the LIN::kSlaveResponse message.
    ///
    /// @param frame            The frame to send in response.
    ///
    void            prepareSlaveResponse(LIN::Frame &frame) 
    {
        _slaveResponse = frame;
    }

    /// Called when a header has been received.
    ///
    /// Must be implemented by the subclass.
    ///
    /// @param fid              The FID from the received header.
    ///
    virtual void    headerReceived(LIN::FID fid);
    
    /// Called when a response requested by requestResponse has been received.
    ///
    /// @param fid              The FID from the header associated with this
    ///                         response.
    /// @param frame            The received response frame.
    ///
    virtual void    responseReceived(LIN::FID fid, LIN::Frame &frame);
    
    /// Called when transmission of a response is completed.
    ///
    /// @param fid              The FID associated with the response that
    ///                         was sent.
    ///
    virtual void    responseSent(LIN::FID fid);

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
    static void     waitBusy();         //< wait while the LIN block is busy

    LIN::FrameID    currentFID() const { return (LIN::FrameID)Lin_get_id(); }

};
