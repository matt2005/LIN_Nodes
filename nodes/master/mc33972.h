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

///@file switches.h

#pragma once

#include <stdint.h>

namespace MC33972
{
enum Input : uint8_t {
    kInputSG0   = 0,
    kInputSG1,
    kInputSG2,
    kInputSG3,
    kInputSG4,
    kInputSG5,
    kInputSG6,
    kInputSG7,
    kInputSG8,
    kInputSG9,
    kInputSG10,
    kInputSG11,
    kInputSG12,
    kInputSG13,
    kInputSP0,
    kInputSP1,
    kInputSP2,
    kInputSP3,
    kInputSP4,
    kInputSP5,
    kInputSP6,
    kInputSP7,
    kInputMax
};

extern void configure();
extern void scan();
extern bool test(uint8_t inp);
extern void analog_select(uint8_t inp);
extern void analog_deselect(uint8_t inp);

extern bool changed;

} //namespace MC33972
