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
    virtual void    st_sleep_requested(SleepType type);

    /// Called when a LIN::kMasterRequest response is received that addresses
    /// this node.
    ///
    /// The default handler deals with kServiceIDReadDataByID, 
    /// kServiceIDWriteDataByID, kServiceIDReadByID/kReadByIDProductID.
    ///
    /// @param frame            The response frame
    /// @return                 If true, the (modified) frame should be sent
    ///                         as a slave response.
    ///
    virtual bool    st_master_request(LIN::Frame &frame);

    /// Prepare a response to the LIN::kSlaveResponse message.
    ///
    /// @param frame            The frame to send in response.
    ///
    void            st_slave_response(const LIN::Frame &frame) 
    {
        _response.copy(frame);
        _sendSlaveResponse = true;
    }

    /// Handler for kServiceIDReadDataByID.
    ///
    /// The default handler deals with requests for page 0 and some fields in page 1.
    ///
    /// @param page             Data page being read
    /// @param index            Data index to be read.
    /// @param value            The parameter's current value.
    /// @return                 True if the parameter is supported and the value has
    ///                         been returned.
    ///
    virtual bool            st_read_data(uint8_t page, uint8_t index, uint16_t &value);

    /// Handler for kServiceIDWriteDataByID
    ///
    /// @param page             Data page being written
    /// @param index            Data index to be written
    /// @param value            Value to be written.
    /// @return                 True if the data was accepted (note that not all
    ///                         writes check errors, data must be read back to be sure).
    ///
    virtual bool            st_write_data(uint8_t page, uint8_t index, uint16_t value);

private:
    Timestamp       _lastActivity;      //< Bus idle timer
    LIN::Frame      _response;          //< canned response frame

    bool            _sendSlaveResponse:1;

    static void     idle_timeout(void *arg); //< idle timeout callback
};
