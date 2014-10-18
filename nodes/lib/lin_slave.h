/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
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
