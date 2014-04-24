///@master.h

#pragma once

#include <stdint.h>

#include "lin_protocol.h"
#include "lin_slave.h"
#include "timer.h"

#include "event.h"
#include "slave.h"

class Master : public SwitchSlave
{
public:
    Master();

    /// Queue a MasterRequest frame.
    ///
    /// @param frame            The frame to send.
    /// @return                 True if the frame is sent, false if it times out.
    ///
    bool            doRequest(LIN::Frame &frame);

    /// Queue a Master Request frame, then a Slave Response to 
    /// gather the slave's reply.
    ///
    /// @param frame            The frame to send, and the buffer into which
    ///                         the response will be placed.
    /// @return                 True if a response is received, false if sending
    ///                         the reequest or receiving the response times out.
    ///
    bool            doRequestResponse(LIN::Frame &frame);

protected:
    virtual void    headerReceived(LIN::FID fid) override;
    virtual void    responseReceived(LIN::FID fid, LIN::Frame &frame) override;
    virtual void    sleepRequested(SleepType type);

private:
    Timer           _eventTimer;
    Timer           _requestTimer;

    // master-mode events
    Event           _controlsRequest;
    Event           _masterRequest;
    Event           _slaveResponse;

    LIN::Frame      * volatile _requestFrame;
    LIN::Frame      * volatile _responseFrame;

    /// Internal waiter for doRequest/doRequestResponse.
    ///
    bool            waitRequest();
};
