
#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer.h"

Timer *Timer::_first;

Timer::Timer(Callback callback, void *arg) :
    _callback(callback),
    _arg(arg),
    _remaining(0),
    _interval(0),
    _expired(false),
    _next(_first)
{
    _first = this;
}

Timer::Timer(uint16_t interval, Callback callback, void *arg) :
    _callback(callback),
    _arg(arg),
    _remaining(interval),
    _interval(interval),
    _expired(false),
    _next(_first)
{
    _first = this;
}

void
Timer::init()
{
    // Reset Timer0
    TIMSK0 = 0;     // interrupts off
    TCCR0B = 0;     // timer off
    TCCR0A = 0;     // output off
    TCNT0 = 0;      // reset counter
    OCR0A = 0;      // clear the compare value

    // Configure for a 1ms tick
    TIFR0 = ((1 << OCF0A) | (1 << TOV0));   // interrupt on overflow / reset
    TCCR0A = 0x02;                          // output off, mode 2
    TCCR0B = 0x03;                          // prescaler divide by 32
    OCR0A = (F_CPU / 1000 / 32) - 1;        // tick every 1ms

    TIMSK0 |= 1 << OCIE0A;                  // compare interrupt on
}

ISR(TIMER0_COMPA_vect)
{
    Timer::tick();
}

void
Timer::tick()
{
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

            // callback?
            if (t->_callback != nullptr) {
                t->_callback(t->_arg);
            } else {
                // mark expired
                t->_expired = true;
            }
            break;

        default:
            // simply decrement
            t->_remaining--;
            break;
        }
        t = t->_next;
    }
}
