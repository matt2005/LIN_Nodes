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


#include <avr/power.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "softserial.h"
#include "board.h"

#ifdef pinDebugTX

// 57600
#if F_CPU == 8000000
# define BIT_DELAY  16
#elif F_CPU == 16000000
# define BIT_DELAY  36
#endif

void
Serial::write(uint8_t c)
{
    // do minimal init - we might be called at any time
    clock_prescale_set(clock_div_1);
    pinDebugTX.set();
    pinDebugTX.cfg_output();

    tx(c);
    wdt_reset();
    Board::us_delay(200);
}

void
Serial::tx(uint8_t c)
{
    uint8_t sreg = SREG;
    cli();

    // start bit
    pinDebugTX.clear();
    tuned_delay(BIT_DELAY);

    // shift data bits
    for (uint8_t mask = 1; mask != 0; mask <<= 1) {
        if (c & mask) {
            pinDebugTX.set();

        } else {
            pinDebugTX.clear();
        }

        tuned_delay(BIT_DELAY);
    }

    // stop bit
    pinDebugTX.set();
    tuned_delay(BIT_DELAY);

    SREG = sreg;
}

void
Serial::tuned_delay(uint16_t delay)
{
    uint8_t tmp = 0;

    asm volatile(
        "sbiw    %0, 0x01   \n\t"
        "ldi     %1, 0xFF   \n\t"
        "cpi     %A0, 0xFF  \n\t"
        "cpc     %B0, %1    \n\t"
        "brne    .-10       \n\t"
        : "+r"(delay), "+a"(tmp)
        : "0"(delay)
    );
}

#else
# error No pinDebugTX defined for this board
#endif
