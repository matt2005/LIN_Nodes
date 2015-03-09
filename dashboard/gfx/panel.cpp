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

/*
 * Panel driver.
 */

#include "panel.h"

extern Panel gPanel;

/*
 * LED-on times in microseconds for 1LSB at each of the possible brightness levels.
 *
 * Note that it takes ~50us to clock in a row pair, during which time the LEDs are off.
 * Minimum actual LED on time is dependent on interrupt latency; probably several microseconds.
 */

Panel::Panel() :
    _timer(&Panel::tick, this),
    _fb_available(kBuf0),
    _fb_ready(kNoBuf),
    _fb_active(kBuf1),
    _dim_level(0)
{
    _phase.counter = 0;
    line_init();
    _timer.call_after(5000);     // wait a few milliseconds before we start
}

/*
 * Notes on panel updates.
 *
 * A complete panel update should be scheduled to take ~16ms. Each update is
 * PaletteEntry::depth time slots for each of the _rows/2 row pairs, with the time slot sizing in powers
 * of 2 (XXX tbd scaling) matching the significance of each bit.
 *
 * Each time slot gets two ticks at brightness levels below max; one at the start
 * to turn the LEDs on, and one interior to the slot to turn it off. At max brightness
 * the slots simply run together.
 *
 * The phase is encoded as:
 *
 * 6543210
 * SSSRRRT
 *
 * Where T is 0 for the on toggle and 1 for the off toggle, RRR is the 3-bit current
 * row number, and SSS is the slot number from 0 .. PaletteEntry::depth - 1.
 *
 */
void
Panel::tick(void *arg)
{
    reinterpret_cast<Panel *>(arg)->_tick();
}

void
Panel::_tick()
{
    if (_phase.is_dimming) {

        line_off();

    } else {
        line_update(_phase.row, _phase.slot);
    }

    /* advance to the next phase */
    Timer::Interval interval = phase_advance();

    /* schedule the next tick */
    _timer.call_after(interval);
}

Timer::Interval
Panel::phase_advance()
{
    unsigned bit_period = _max_brightness << _phase.slot;
    unsigned on_time = bit_period >> _dim_level;
    Timer::Interval interval;

    /* interval to next tick is based on current phase & brightness */
    if (_phase.is_dimming) {
        /* dimming phase - consume the remainder of the slot time */
        interval = bit_period - on_time;

    } else {
        /* display phase - time based on brightness */
        interval = on_time;
    }

    /* at max brightness, skip the dimming phase */
    _phase.counter += (_dim_level > 0) ? 1 : 2;

    /* at phase wrap, we are in either the display or dimming phase of the last row */
    if (_phase.slot >= PaletteEntry::depth) {
        _phase.counter = 0;

        /* ready to flip framebuffers? */
        if (_fb_ready != kNoBuf) {

            /* active buffer -> available */
            _fb_available = _fb_active;

            /* ready buffer -> active */
            _fb_active = _fb_ready;
            _fb_ready = kNoBuf;
        }
    }

    return interval;
}

