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

class Timer
{
public:
    typedef void (* Callback)(void *arg);
    typedef uint64_t        Time;
    typedef uint32_t        Interval;
    static const Time       endOfTime   = ~0ULL;

    Timer(Callback callback, void *arg) :
        _callback(callback),
        _arg(arg)
    {
    }

    static Time             time();
    static void             run();
    static void             init();

    static Interval         time_since(Time then) { return time() - then; }

    void                    call_after(Interval after) { call_at(time() + after); }
    void                    call_at(Time deadline);
    void                    cancel();

private:
    static const Interval   _minInterval    = 20;
    static const Interval   _maxInterval    = 0x8fffffff;

    static Timer            *_list;
    static uint32_t         _highWord;
    static bool             _running;

    static void             reschedule();

    Time                    _deadline   = 0;
    Timer                   *_next      = nullptr;
    Callback                _callback   = nullptr;
    void                    *_arg       = nullptr;
};

class Ticker : public Timer
{
public:
    Ticker(Timer::Interval interval) :
        Timer(Ticker::tick, this),
        _interval(interval),
        _ticked(false)
    {
        call_after(interval);
    }

    bool                    did_tick() volatile;

private:
    static void             tick(void *arg);

    Timer::Interval         _interval;
    volatile bool           _ticked;

    void                    _tick();
};
