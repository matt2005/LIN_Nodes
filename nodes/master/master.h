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

    /// Send a Master Request frame, then a Slave Response to
    /// gather the slave's reply.
    ///
    /// @param frame            The frame to send, and the buffer into which
    ///                         the response (if any) will be placed.
    ///
    void            do_request_response(LIN::Frame &frame);

    /// Enable / disable sleep
    ///
    /// @param enable           If true, sleep is enabled. The master will enter
    ///                         sleep mode at the end of the current cycle
    ///                         if it is not in config mode.
    ///                         If false, sleep is inhibited.
    ///
    void            set_sleep_enable(bool enable)
    {
        _sleepRequest = enable;

        if (!enable) {
            _awake = true;
        }
    }

    /// Enable / disable tester mode
    ///
    /// @param enable           If true, inhibits sleep and enables support for the
    ///                         tester in the schedule.
    ///
    void            set_tester_present(bool enable)
    {
        _testerPresent = enable;
    }

protected:
    virtual void    header_received(LIN::FrameID fid) override;
    virtual void    response_received(LIN::FrameID fid, LIN::Frame &frame) override;

private:
    static const LIN::FrameID _schedule[];
    static const uint8_t      _scheduleLength;

    Timer           _eventTimer;
    uint8_t         _eventIndex;
    LIN::Frame      *volatile _requestFrame;
    LIN::Frame      *volatile _responseFrame;
    uint8_t         _configParam;

    volatile bool   _sendConfigResponseFrame: 1;
    bool            _sleepRequest: 1;
    bool            _awake: 1;
    bool            _testerPresent: 1;

    static LIN::FrameID schedule_entry(uint8_t idx)
    {
        return (LIN::FrameID)pgm_read_byte(&_schedule[idx]);
    }

    /// Event initiator
    static void     event(void *arg);
    void            _event();

    /// Config request handler
    ///
    void            handle_config_request(LIN::ConfigFrame &frame);
    void            handle_config_response();
};

extern Master gMaster;
