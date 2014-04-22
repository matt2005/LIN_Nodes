///@file timer.h
#pragma once

#include <stdint.h>

/// General-purpose periodic/one-shot timer
///
class Timer
{
public:
    typedef void        (*Callback)(void *arg);  //< timer expiry callback function
    typedef uint16_t    Period;         //< timer period in milliseconds

    static const Period kMaxPeriod = ~(Period)0; // UINT16_MAX

    /// Create a one-shot timer.
    ///
    /// @param callback         If set, the callback function to call when
    ///                         the timeout expires.
    /// @param arg              Passed to the callback function.
    ///
    Timer(Callback callback = nullptr, void *arg = nullptr);

    /// Create a periodic timer.
    ///
    /// @param interval         The interval in ticks.
    /// @param callback         If set, the callback function to call when
    ///                         the timeout expires.
    /// @param arg              Passed to the callback function.
    ///
    Timer(Period interval, Callback callback = nullptr, void *arg = nullptr);

    /// Set the delay until the timer expires.
    ///
    /// Note that a value of 1 means that it will elapse on the next tick, and a value of
    /// 0 will disable the timer.
    ///
    /// @param ticks            The number of timer ticks to wait before the timer expires.
    ///
    void                setRemaining(Period ticks) { _remaining = ticks; _expired = false; };

    /// Adjust the interval between periodic timer expiry.
    ///
    /// @param ticks            The number of ticks to be reloaded on the next timer expiration.
    ///
    void                setInterval(Period ticks) { _interval = ticks; }

    /// Tests whether a timer has expired.
    ///
    /// Timers without callbacks can use this to check whether a timer's period has
    /// expired. Calling this function resets the expired status.
    ///
    /// @return                 True if the timer's delay has expired.
    ///
    bool                didExpire() { bool didExpire = _expired; _expired = false; return didExpire; }

    /// Iterate the set of timers, decrement their remaining counts and
    /// call any applicable callbacks.
    ///
    static void         tick();

private:
    const Callback      _callback;  //< callback function or nullptr if no callback
    void                * const _arg;

    volatile Period     _remaining; //< number of ticks remaining before expiry
    volatile Period     _interval;  //< reload value for periodic, 0 for one-shot
    volatile bool       _expired;   //< true if the timeout has expired

    Timer               *_next;     //< list linkage
    static Timer        *_first;    //< list anchor

    /// Initialise the timer system
    ///
    static void         init();

};
