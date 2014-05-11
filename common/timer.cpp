
#include <avr/io.h>
#include <avr/interrupt.h>

#include "board.h"
#include "timer.h"

volatile Timer::Timeval Timer::_now = 0;
Timer *Timer::_first = nullptr;

Timer::Timer(Callback callback, void *arg, uint16_t interval) :
    _callback(callback),
    _arg(arg),
    _remaining(interval),
    _interval(interval),
    _next(_first)
{
    if (_first == nullptr) {

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
    _first = this;
}

ISR(TIMER0_COMPA_vect)
{
    Timer::tick();
}

void
Timer::tick()
{
    Timer *t = _first;

    _now++;

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

Decrementer *Decrementer::_first;

Decrementer::Decrementer(Timer::Timeval initialCount) :
    _count(initialCount),
    _next(_first)
{
    if (_first == nullptr) {
        _first = this;
    }
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
