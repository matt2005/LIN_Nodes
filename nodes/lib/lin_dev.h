///@file lin_dev.h

#pragma once

#include "util.h"
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
    void            isr_TC();
    
    /// Called from the error ISR
    ///
    void            isr_error();

    // test mode
    void            master_test();

    enum Error : uint8_t {
        kErrorLine,                       //< readback error when transmitting
        kErrorChecksum,                   //< received data checksum mismatch
        kErrorParity,                     //< header parity error
        kErrorFraming,                    //< framing error
        kErrorSynchronisation,            //< bitrate synchronisation error
        kErrorProtocol,                   //< slave protocol error
        kErrorSlave1,                     //< slave private error 1
        kErrorSlave2,                     //< slave private error 2
        kErrorMax
    };

    Util::Counter8  errors[kErrorMax];     //< error counters

protected:
    /// Ask the driver to pay attention to the next response that it sees.
    ///
    /// This will usually be the response associated with an interesting
    /// header.
    ///
    /// @param length           The length of the response to be received.
    ///
    void            expect_response(uint8_t length);
    
    /// Send a LIN header. This is strictly speaking a master function, but
    /// it shares functionality with the slave.
    ///
    /// @param fid              The Frame ID to send in the header.
    ///
    void            send_header(LIN::FrameID fid);

    /// Send a response.
    ///
    /// Subclass implementation of header_received will call when it wants to
    /// send a response for an interesting header.
    ///
    /// Responses are always sent in LIN 2.x format, unless the FID from the
    /// header implies 1.x (i.e. SlaveResponse).
    ///
    /// @param frame            The response frame to send.
    /// @param length           The length of the response frame.
    ///
    void            send_response(const LIN::Frame &frame, uint8_t length);

    void            send_response(const volatile LIN::Frame &frame, uint8_t length)
    {
        send_response(const_cast<const LIN::Frame&>(frame), length);
    }

    /// Called when a header has been received.
    ///
    /// Must be implemented by the subclass.
    ///
    /// @param fid              The FID from the received header.
    ///
    virtual void    header_received(LIN::FrameID fid);
    
    /// Called when a response requested by expect_response has been received.
    ///
    /// @param fid              The FID from the header associated with this
    ///                         response.
    /// @param frame            The received response frame.
    ///
    virtual void    response_received(LIN::FrameID fid, LIN::Frame &frame);
    
    /// Called when a response has been sent
    ///
    virtual void    response_sent();

    /// Wait until the LBUSY bit is cleared
    ///
    static void     wait_busy() { while (LINSIR & (1 << LBUSY)) {} }

    /// Fetch the current FID from the hardware
    ///
    static LIN::FrameID current_FrameID() { return (LIN::FrameID)Lin_get_id(); }

private:
};

