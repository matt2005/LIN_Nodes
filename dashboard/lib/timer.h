/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
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
