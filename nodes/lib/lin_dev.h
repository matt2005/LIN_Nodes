/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

///@file lin_dev.h

#pragma once

#include "util.h"
#include "lin_drv.h"
#include "lin_defs.h"

class LINDev
{
public:
    static const bool   kLINDevInterrupts = false;
    static const bool   kLINDevPolled = true;

    LINDev(bool polled = kLINDevInterrupts);

    /// Do one-time block initialisation
    ///
    void            init();

    /// Re-initialise the block
    ///
    void            reinit();

    /// Perform periodic actions; polls for new activity in polled
    /// mode.
    ///
    virtual void    tick();

    /// Called from the transfer-complete ISR
    ///
    void            isr_TC();

    /// Called from the error ISR
    ///
    void            isr_error();

    enum Error : uint8_t {
        kErrorLine,                       //< readback error when transmitting
        kErrorChecksum,                   //< received data checksum mismatch
        kErrorParity,                     //< header parity error
        kErrorFraming,                    //< framing error
        kErrorSynchronisation,            //< bitrate synchronisation error
        kErrorProtocol,                   //< slave protocol error
        kErrorMax
    };

    Util::Counter16  errors[kErrorMax];     //< error counters

    static_assert((Generic::kParamProtocol - Generic::kParamLine) == (kErrorMax - 1), "LIN error definitions out of sync");

protected:
    /// Send a LIN header from the master task.
    ///
    /// @param fid              The Frame ID to send in the header.
    ///
    void            mt_send_header(uint8_t fid);

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
    /// @param resp             The response to send.
    /// @param length           The length of the response.
    ///
    void            st_send_response(const Response &resp, uint8_t length = 8);

    void            st_send_response(const volatile Response &resp, uint8_t length = 8)
    {
        st_send_response(const_cast<const Response &>(resp), length);
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
    /// @param resp             The received response.
    ///
    virtual void    st_response_received(Response &resp);

    /// Called when a response has been sent.
    ///
    /// Can be used to disable the LIN transceiver after transmission is complete.
    ///
    virtual void    st_response_sent();

    /// Fetch the current FID from the hardware
    ///
    static uint8_t current_FrameID() { return (uint8_t)Lin_get_id(); }

private:
    bool            _polled: 1;
    bool            _responseCopyBack: 1;
};

