/*
 * Copyright (c) 2012-2015, Mike Smith, <msmith@purgatory.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * o Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

///@file lin_slave.h

///
/// LIN driver wrapper for slaves
///

#pragma once

#include "lin_dev.h"
#include "timer.h"
#include "board.h"

class Slave : public LINDev
{
public:
    /// Constructor
    ///
    /// @param nad          Node address this slave will take.
    /// @param polled       Passed to LINDev.
    ///
    Slave(uint8_t nad = 0xff, bool polled = kLINDevInterrupts);

    virtual void    tick() override;

    /// @return True if the network is considered 'awake'
    bool            is_awake() const { return !_lastActivity.is_older_than(100); }

protected:

    enum SleepType {
        kSleepTypeRequested,
        kSleepTypeIdle
    };
    static const uint16_t kIdleTimeout = 4000;  //< 4 seconds

    uint8_t         _nad;               //< node address
    Timestamp       _lastActivity;      //< Bus idle timer

    virtual void    st_header_received() override;
    virtual void    st_response_received(Response &resp) override;

    /// Called when the network is told to sleep.
    ///
    /// @param type             Indicates whether this is a requested or idle
    ///                         timeout sleep request.
    ///
    virtual void    st_sleep_requested(SleepType type);

    /// Called when a LIN::kMasterRequest response is received that addresses
    /// this node.
    ///
    /// The default handler deals with kServiceIDReadDataByID,
    /// kServiceIDWriteDataByID, kServiceIDReadByID/kReadByIDProductID.
    ///
    /// @param resp             The response.
    /// @return                 If true, the (modified) response should be sent
    ///                         as a slave response.
    ///
    virtual bool    st_master_request(Response &resp);

    /// Prepare a response to the LIN::kSlaveResponse message.
    ///
    /// @param resp            The frame to send in response.
    ///
    void            st_slave_response(const Response &resp)
    {
        _response = resp;
        _sendSlaveResponse = true;
    }

    /// Handler for kServiceIDReadDataByID.
    ///
    /// The default handler deals with requests for page 0 and some fields in page 1.
    ///
    /// @param addresss         address being read
    /// @param value            the parameter's current value.
    /// @return                 true if the parameter is supported and the value has
    ///                         been returned.
    ///
    virtual bool    st_read_data(Parameter::Address, uint16_t &value);

    /// Handler for kServiceIDWriteDataByID
    ///
    /// @param address          address being written
    /// @param value            value to be written.
    /// @return                 true if the data was accepted (note that not all
    ///                         writes check errors, data must be read back to be sure).
    ///
    virtual bool    st_write_data(Parameter::Address, uint16_t value);

    /// Reformat a service request as an error response
    ///
    void            st_error_response(Response &resp, uint8_t err);

private:
    Response        _response;          //< canned response frame

    bool            _sendSlaveResponse: 1;

    static void     idle_timeout(void *arg); //< idle timeout callback
};
