/*
 * Copyright (c) 2012-2015, Mike Smith, <msmith@purgatory.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * o Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <stdint.h>
#include "common/lin_protocol.h"
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
    Datum   airFuelRatio;

    Datum   fuelLevel;

private:
    enum State {
        waitBreak,
        waitSynch,
        waitHeader,
        waitResponse
    };

    uint8_t             _buf[9];
    uint8_t             _receivedLen    = 0;
    uint8_t             _waitLen        = 0;
    uint8_t             _fid            = kFrameIDNone;
    State               _state          = waitBreak;

    PerfCounter         _perfFrames;

    void                header_received();
    void                response_received();

    bool                frame_bit(uint8_t index) const { return _buf[index / 8] & (1 << (index % 8)); }
};

extern LINDev gLIN;
