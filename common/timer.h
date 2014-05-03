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
    ///                         timer started with setRemaining.
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
    void                setRemaining(Timeval ticks) { _remaining = ticks; };

    /// Adjust the interval between periodic timer expiry.
    ///
    /// @param ticks            The number of ticks to be reloaded on the next timer expiration.
    ///
    void                setInterval(Timeval ticks) { _interval = ticks; }



    /// Returns the current time.
    ///
    /// Note that due to the limited range of Timeval, time wraps every ~65 seconds.
    ///
    /// @return                 The current time in milliseconds
    ///
    static Timeval      timeNow() { return _now; }

    /// Returns the delta beetween some time in the past and now.
    ///
    /// Note that due to the limited range of Timeval, intervals greater
    /// than ~65 seconds will be truncated.
    ///
    /// @param then             The time in the past
    /// @return                 The time difference in milliseconds
    ///
    static Timeval      timeSince(Timeval then) { return _now - then; }

    /// Iterate the set of timers, decrement their remaining counts and
    /// call any applicable callbacks.
    ///
    static void         tick();

protected:
    const Callback      _callback;  //< callback function
    void               *const _arg;      //< callback function argument

    volatile Timeval    _remaining; //< number of ticks remaining before expiry
    volatile Timeval    _interval;  //< reload value for periodic, 0 for one-shot

    Timer               *_next;     //< list linkage

    static Timer        *_first;    //< list anchor
    static volatile Timeval _now;
};
