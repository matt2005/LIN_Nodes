///@file lin_dev.h

#pragma once

#include "util.h"
#include "lin_drv.h"
#include "lin_protocol.h"

class LINDev
{
public:
    LINDev();
    
    /// Do one-time block initialisation
    ///
    static void     init();

    /// Re-initialise the block
    ///
    static void     reinit();

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
    /// Send a LIN header from the master task.
    ///
    /// @param fid              The Frame ID to send in the header.
    ///
    static void     mt_send_header(LIN::FrameID fid);

    /// Called by the slave task from st_st_header_received when it wants the
    /// response associated with a header.
    ///
    /// This will usually be the response associated with an interesting
    /// header.
    ///
    /// @param length           The length of the response to be received.
    ///
    void            st_expect_response(uint8_t length = 8);
    
    /// Called by the slave task from st_st_header_received when it wants to
    /// send a response.
    ///
    /// Responses are always sent in LIN 2.x format, unless the FID from the
    /// header implies 1.x (i.e. SlaveResponse).
    ///
    /// @param frame            The response frame to send.
    /// @param length           The length of the response frame.
    ///
    void            st_send_response(const LIN::Frame &frame, uint8_t length);

    void            st_send_response(const volatile LIN::Frame &frame, uint8_t length)
    {
        st_send_response(const_cast<const LIN::Frame&>(frame), length);
    }

    /// Called when a header has been received.
    ///
    /// Must be implemented by the subclass.
    ///
    /// @param fid              The FID from the received header.
    ///
    virtual void    st_header_received();
    
    /// Called when a response requested by st_expect_response has been received.
    ///
    /// @param fid              The FID from the header associated with this
    ///                         response.
    /// @param frame            The received response frame.
    ///
    virtual void    st_response_received(LIN::Frame &frame);
    
    /// Called when a response has been sent.
    ///
    /// Can be used to disable the LIN transceiver after transmission is complete.
    ///
    virtual void    st_response_sent();

    /// Fetch the current FID from the hardware
    ///
    static LIN::FrameID current_FrameID() { return (LIN::FrameID)Lin_get_id(); }

private:
};

