///@file lin_dev.h

#pragma once

#include "lin_drv.h"
#include "lin_protocol.h"

class LINDev
{
public:
    LINDev();
    
    /// Initialise the block
    ///
    void            init();

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
        kErrSlave1,                     //< slave private error 1
        kErrSlave2,                     //< slave private error 2
        kErrMax
    };

    uint16_t        errors[kErrMax];     //< error counters

protected:
    /// Ask the driver to pay attention to the next response that it sees.
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
    /// Subclass implementation of headerReceived will call when it wants to
    /// send a response for an interesting header.
    ///
    /// Responses are always sent in LIN 2.x format, unless the FID from the
    /// header implies 1.x (i.e. SlaveResponse).
    ///
    /// @param frame            The response frame to send.
    /// @param length           The length of the response frame.
    ///
    void            sendResponse(const LIN::Frame &frame, uint8_t length);

    void            sendResponse(const volatile LIN::Frame &frame, uint8_t length)
    {
        sendResponse(const_cast<const LIN::Frame&>(frame), length);
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
    
    /// Called when a response has been sent
    ///
    virtual void    responseSent();

    /// Wait until the LBUSY bit is cleared
    ///
    void            waitBusy() const { while (LINSIR & (1 << LBUSY)) {} }

    /// Fetch the current FID from the hardware
    ///
    LIN::FrameID    currentFID() const { return (LIN::FrameID)Lin_get_id(); }

private:
};

