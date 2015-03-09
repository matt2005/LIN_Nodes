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

#include "graphics.h"
#include "lpc111x.h"

class Panel;

/* abstract panel output driver */
class PanelOut
{
public:
    virtual void    line_off() = 0;
    virtual void    line_update(unsigned row, unsigned slot, FrameBuffer *buffer) = 0;
};

class PanelV2PIO : public PanelOut
{
public:
    PanelV2PIO();
    virtual void    line_off() override;
    virtual void    line_update(unsigned row, unsigned slot, FrameBuffer *buffer) override;
private:
    static const uint16_t   _bR1 = GPIO_IO_P0;
    static const uint16_t   _bG1 = GPIO_IO_P1;
    static const uint16_t   _bB1 = GPIO_IO_P2;
    static const uint16_t   _bR2 = GPIO_IO_P6;
    static const uint16_t   _bG2 = GPIO_IO_P7;
    static const uint16_t   _bB2 = GPIO_IO_P8;
    static const uint16_t   _bA = GPIO_IO_P3;   // to be shared with R1
    static const uint16_t   _bB = GPIO_IO_P9;   // to be shared with G1
    static const uint16_t   _bC = GPIO_IO_P11;  // to be shared with B1
    static const uint16_t   _bD = _bR2;         // shared with R2

    static const uint16_t   _bCLK = GPIO_IO_P4;
    static const uint16_t   _bLAT = GPIO_IO_P5;
    static const uint16_t   _bOE = GPIO_IO_P8;
};
