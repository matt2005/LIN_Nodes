#pragma once

#include <stdint.h>
#include "common/protocol.h"

class LINDev
{
public:
    LINDev(unsigned bitrate);

    static void do_interrupt();

    void        interrupt();

    volatile bool       warnSummary     = false;
    volatile bool       errorSummary    = false;

    volatile bool       ttLeftTurn      = true;
    volatile bool       ttRightTurn     = true;
    volatile bool       ttLowBeam       = true;
    volatile bool       ttHighBeam      = true;
    volatile bool       ttFogLights     = true;

    volatile unsigned   roadSpeed       = 35;
    volatile unsigned   engineRPM       = 8765;
    volatile unsigned   batteryVoltage  = 131;
    volatile unsigned   waterTemperature = 172;
    volatile unsigned   oilPressure     = 45;
    volatile unsigned   fuelLevel       = 20;

private:
    enum State {
        waitBreak,
        waitSynch,
        waitHeader,
        waitResponse
    };

    uint8_t             _buf[9];
    unsigned            _receivedLen    = 0;
    unsigned            _waitLen        = 0;
    uint8_t             _fid            = LIN::kFIDNone;
    State               _state          = waitBreak;

    void                headerReceived();
    void                responseReceived();

    bool                frameBit(uint8_t index) const { return _buf[index / 8] & (1 << (index % 8)); }
};
