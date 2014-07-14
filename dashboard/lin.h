#pragma once

#include <stdint.h>
#include "common/protocol.h"
#include "perf.h"
#include "datum.h"

class LINDev
{
public:
    LINDev(unsigned bitrate);

    void        interrupt();

    Datum   warnSummary;
    Datum   errorSummary;
    Datum   linkUp;

    Datum   ttLeftTurn;
    Datum   ttRightTurn;
    Datum   ttLowBeam;
    Datum   ttHighBeam;
    Datum   ttFogLights;

    Datum   roadSpeed;
    Datum   engineRPM;
    Datum   batteryVoltage;
    Datum   waterTemperature;
    Datum   oilPressure;
    Datum   fuelLevel;
    Datum   airFuelRatio;

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
