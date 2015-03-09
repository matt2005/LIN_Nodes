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

#include <avr/io.h>
#include <avr/interrupt.h>

#include "board.h"
#include "timer.h"
#include "util.h"

volatile Timer::Timeval Timer::_now;
Timer *Timer::_first;

Timer::Timer(Callback callback, void *arg, uint16_t interval) :
    _callback(callback),
    _arg(arg),
    _remaining(interval),
    _interval(interval),
    _next(_first)
{
    _first = this;
    init();
}

void
Timer::init()
{
    // Check whether the timer is already configured
    if (TIMSK0 & (1 << OCIE0A)) {
        return;
    }

    // Reset Timer0
    TIMSK0 = 0;     // interrupts off
    TCCR0B = 0;     // timer off
    TCCR0A = 0;     // output off
    TCNT0 = 0;      // reset counter
    OCR0A = 0;      // clear the compare value
    ASSR = 0;

    // Configure for a 1ms tick
    TIFR0 = ((1 << OCF0A) | (1 << TOV0));   // interrupt on overflow / reset
    TCCR0A = 0x02;                          // output off, mode 2
    OCR0A = (F_CPU / 1000 / 64);            // tick every 1ms
    TCCR0B = 0x04;                          // prescaler divide by 64 and timer on

    TIMSK0 |= 1 << OCIE0A;                  // compare interrupt on
}

#ifdef WITH_TIMER_PREEMPTION
ISR(TIMER0_COMPA_vect, ISR_NOBLOCK)
#else
ISR(TIMER0_COMPA_vect)
#endif
{
    Timer::tick();
}

Timer::Timeval
Timer::time_now()
{
    return Util::intsafe_copy(&_now);
}

Timer::Timeval
Timer::time_since(Timeval then)
{
    return time_now() - then;
}

void
Timer::tick()
{
    // As a special case, it's OK to touch _now here directly because we
    // are the only writer.
    _now++;

    Timer *t = _first;

    while (t != nullptr) {
        switch (t->_remaining) {
        case 0:
            // disabled, do nothing.
            break;

        case 1:
            // expiring on this tick

            // reload if periodic, otherwise set to 0
            t->_remaining = t->_interval;

            // call the callback
            t->_callback(t->_arg);
            break;

        default:
            // simply decrement
            t->_remaining--;
            break;
        }

        t = t->_next;
    }

    Decrementer::tick();
}

void
Ticker::tick(void *arg)
{
    (reinterpret_cast<Ticker *>(arg))->_ticked = true;
}

Timestamp::Timestamp() : _taken(Timer::time_now())
{
    Timer::init();
}

Timer::Timeval
Timestamp::time() const
{
    return Util::intsafe_copy(&_taken);
}

Timer::Timeval
Timestamp::time_since() const
{
    // avoid having _taken updated after the current time
    auto taken = time();
    return Timer::time_since(taken);
}

bool
Timestamp::is_older_than(Timer::Timeval interval) const
{
    return time_since() > interval;
}

Decrementer *Decrementer::_first;

Decrementer::Decrementer(Timer::Timeval initialCount) :
    _count(initialCount),
    _next(_first)
{
    _first = this;
    Timer::init();
}

void
Decrementer::tick()
{
    Decrementer *d = _first;

    while (d != nullptr) {
        if (d->_count > 0) {
            d->_count--;
        }

        d = d->_next;
    }
}
