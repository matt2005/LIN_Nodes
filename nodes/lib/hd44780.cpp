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


#include <avr/wdt.h>

#include "board.h"

#include "hd44780.h"

void
HD44780::init()
{
    // configure LCD interface pins
    portLCD.DDR = 0x7f;
    portLCD.set(0);

    // wait to allow display to boot
    Board::ms_delay(100);

    // configure display for 4-bit mode
    // http://web.alfredstate.edu/weimandn/index.html
    send_nibble(0x3);
    Board::ms_delay(10);

    send_nibble(0x3);
    Board::us_delay(200);

    send_nibble(0x3);
    Board::us_delay(200);

    send_nibble(0x2);
    Board::us_delay(80);

    // configure for 2 lines
    send_cmd(0x28);
    Board::us_delay(80);

    // display off (?)
    send_cmd(0x08);
    Board::us_delay(80);

    // clear display & home cursor
    send_cmd(0x01);
    Board::ms_delay(4);

    // set cursor direction
    send_cmd(0x06);
    Board::us_delay(80);

    // turn on display
    send_cmd(0x0c);
    Board::us_delay(80);
}

void
HD44780::clear()
{
    // clear display & home cursor
    send_cmd(0x01);
    Board::ms_delay(4);
}

void
HD44780::move(Position p)
{
    uint8_t cmd = 0x80;

    if (p.y & 1) {
        cmd |= 0x40;
    }

    if (p.y & 2) {
        cmd |= 0x14;
    }

    send_cmd(cmd + p.x);
}

void
HD44780::write(uint8_t c)
{
    send_nibble((c >> 4)  | bitDnC);
    send_nibble((c & 0xf) | bitDnC);
    Board::us_delay(80);
}

void
HD44780::send_nibble(uint8_t val)
{
    portLCD.set(val);
    portLCD.set(val | bitE);
    Board::us_delay(10);
    portLCD.set(val);
    Board::us_delay(10);
}

void
HD44780::send_cmd(uint8_t cmd)
{
    send_nibble(cmd >> 4);
    send_nibble(cmd & 0xf);
    Board::us_delay(80);
}
