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

    /// Enable / disable programmer mode
    ///
    /// @param enable           If true, adjust the schedule to permit the programmer
    ///                         to issue requests to nodes on the bus.
    ///
    void            set_programmer_mode(bool enable)
    {
        _programmerMode = enable;
        set_sleep_enable(false);
    }

protected:
    virtual void    header_received(LIN::FrameID fid) override;
    virtual void    response_received(LIN::FrameID fid, LIN::Frame &frame) override;

private:
    static const LIN::FrameID _normalSchedule[];
    static const uint8_t      _normalScheduleLength;
    static const LIN::FrameID _configSchedule[];
    static const uint8_t      _configScheduleLength;

    Timer           _eventTimer;
    uint8_t         _eventIndex;
    LIN::Frame       *volatile _requestFrame;
    LIN::Frame       *volatile _responseFrame;
    uint8_t         _configParam;

    volatile bool   _sendConfigResponseHeader: 1;
    volatile bool   _sendConfigResponseFrame: 1;
    bool            _sleepEnable: 1;
    bool            _sleepActive: 1;
    bool            _programmerMode: 1;

    uint8_t         schedule_length() const
    {
        return _programmerMode ? _configScheduleLength : _normalScheduleLength;
    }

    LIN::FrameID    schedule_entry(uint8_t idx) const
    {
        return (LIN::FrameID)pgm_read_byte(_programmerMode ?
                                           &_configSchedule[idx] :
                                           &_normalSchedule[idx]);
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
