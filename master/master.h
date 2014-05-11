///@master.h

#pragma once

#include <stdint.h>

#include "lin_protocol.h"
#include "lin_dev.h"
#include "timer.h"

class Master : public LINDev
{
public:
    Master();

    volatile LIN::RelayFrame relayFrame;
    volatile LIN::Frame requestResponseFrame;

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

private:
    static const LIN::FrameID normalSchedule[];
    static const LIN::FrameID configSchedule[];

    Timer           _eventTimer;
    uint8_t         _eventIndex;

    uint8_t         _configParam;
    uint8_t         _configDecayTimer;

    volatile bool   _sendRequest:1;
    volatile bool   _getResponse:1;
    volatile bool   _sendConfigResponseHeader:1;
    volatile bool   _sendConfigResponseFrame:1;

    /// Event initiator
    static void     event(void *arg);
    void            _event();

    /// Internal waiter for doRequest/doRequestResponse.
    ///
    bool            waitRequest();

    /// Config request handler
    ///
    void            handleConfigRequest(LIN::ConfigFrame &frame);
    void            handleConfigResponse();
};

extern Master gMaster;
