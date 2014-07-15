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
    bool            do_request(LIN::Frame &frame);

    /// Queue a Master Request frame, then a Slave Response to
    /// gather the slave's reply.
    ///
    /// @param frame            The frame to send, and the buffer into which
    ///                         the response will be placed.
    /// @return                 True if a response is received, false if sending
    ///                         the reequest or receiving the response times out.
    ///
    bool            do_request_response(LIN::Frame &frame);

    /// Enable / disable sleep
    ///
    /// @param enable           If true, sleep is enabled. The master will enter
    ///                         sleep mode at the end of the current schedule loop
    ///                         if it is not in config mode.
    ///                         If false, sleep is inhibited.
    ///
    void            set_sleep_enable(bool enable)
    {
        _sleepEnable = enable;

        if (!enable) {
            _sleepActive = false;
        }
    }


protected:
    virtual void    header_received(LIN::FrameID fid) override;
    virtual void    response_received(LIN::FrameID fid, LIN::Frame &frame) override;

private:
    static const LIN::FrameID normalSchedule[];
    static const LIN::FrameID configSchedule[];

    Timer           _eventTimer;
    uint8_t         _eventIndex;

    uint8_t         _configParam;
    uint8_t         _configDecayTimer;

    volatile bool   _sendRequest: 1;
    volatile bool   _getResponse: 1;
    volatile bool   _sendConfigResponseHeader: 1;
    volatile bool   _sendConfigResponseFrame: 1;
    bool            _sleepEnable: 1;
    bool            _sleepActive: 1;

    /// Event initiator
    static void     event(void *arg);
    void            _event();

    /// Internal waiter for request/expect_response.
    ///
    bool            wait_request();

    /// Config request handler
    ///
    void            handle_config_request(LIN::ConfigFrame &frame);
    void            handle_config_response();
};

extern Master gMaster;
