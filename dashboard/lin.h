#pragma once

#include <stdint.h>
#include "common/protocol.h"
#include "perf.h"

class LINDev
{
public:
    LINDev(unsigned bitrate);

    void        interrupt();

    volatile bool       warnSummary:1;
    volatile bool       errorSummary:1;
    volatile bool       linkUp:1;

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

    PerfCounter         _perfBytes;
    PerfCounter         _perfFrames;

    void                headerReceived();
    void                responseReceived();

    bool                frameBit(uint8_t index) const { return _buf[index / 8] & (1 << (index % 8)); }
};

extern LINDev gLIN;
