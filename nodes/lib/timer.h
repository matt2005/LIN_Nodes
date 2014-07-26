/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

///@file timer.h

#pragma once

#include <stdint.h>

/// General-purpose periodic/one-shot timer
///
class Timer
{
public:
    typedef void        (*Callback)(void *arg);  //< timer expiry callback function
    typedef uint16_t    Timeval;        //< timer period or value in milliseconds

    static const Timeval kMaxTimeval = ~(Timeval)0; // UINT16_MAX

    /// Construct a timer.
    ///
    /// @param callback         If set, the callback function to call when
    ///                         the timeout expires, or nullptr if didExpire will
    ///                         be used.
    /// @param interval         The interval in ticks, or zero for a one-shot
    ///                         timer started with set_remaining.
    /// @param arg              Passed to the callback function.
    ///
    Timer(Callback callback, void *arg = nullptr, Timeval interval = 0);

    /// Set the delay until the timer expires.
    ///
    /// Note that a value of 1 means that it will elapse on the next tick, and a value of
    /// 0 will disable the timer.
    ///
    /// @param ticks            The number of timer ticks to wait before the timer expires.
    ///
    void                set_remaining(Timeval ticks) { _remaining = ticks; };

    /// Adjust the interval between periodic timer expiry.
    ///
    /// @param ticks            The number of ticks to be reloaded on the next timer expiration.
    ///
    void                set_interval(Timeval ticks) { _interval = ticks; }

    /// Returns the current time.
    ///
    /// Note that due to the limited range of Timeval, time wraps every ~65 seconds.
    ///
    /// @return                 The current time in milliseconds
    ///
    static Timeval      time_now() { return _now; }

    /// Returns the delta beetween some time in the past and now.
    ///
    /// Note that due to the limited range of Timeval, intervals greater
    /// than ~65 seconds will be truncated.
    ///
    /// @param then             The time in the past
    /// @return                 The time difference in milliseconds
    ///
    static Timeval      time_since(Timeval then) { return _now - then; }

    /// Iterate the set of timers, decrement their remaining counts and
    /// call any applicable callbacks.
    ///
    static void         tick();

protected:
    const Callback      _callback;  //< callback function
    void               *const _arg; //< callback function argument

    volatile Timeval    _remaining; //< number of ticks remaining before expiry
    volatile Timeval    _interval;  //< reload value for periodic, 0 for one-shot

private:

    Timer               *_next;     //< list linkage

    static Timer        *_first;    //< list anchor
    static volatile Timeval _now;
};

class Ticker : public Timer
{
public:
    Ticker(Timer::Timeval interval) : 
        Timer(Ticker::tick, this, interval),
        _ticked(false)
    {
    }

    bool                did_tick()
    {
        if (_ticked) {
            _ticked = false;
            return true;
        }
        return false;
    }

private:
    volatile bool       _ticked;
    static void         tick(void *arg);
};

class Timestamp
{
public:
    Timestamp() : _taken(Timer::time_now()) {}

    Timer::Timeval  time_since() const { return Timer::time_since(_taken); }
    bool            is_older_than(Timer::Timeval interval) const { return time_since() > interval; }
    void            update() { _taken = Timer::time_now(); }

private:
    volatile Timer::Timeval _taken;
};

class Decrementer
{
public:
    Decrementer(Timer::Timeval initialCount = 0);

    void        set_milliseconds(uint16_t msec) { _count = msec; }
    void        set_seconds(uint8_t sec)        { _count = sec * 1000U; }
    void        clear()                        { _count = 0; }
    bool        expired() const { return _count == 0; }

    static void tick();

private:
    volatile Timer::Timeval _count;
    Decrementer         *_next;

    static Decrementer  *_first;
};
