#include "timer.h"
#include "lpc111x.h"

class CriticalSection
{
public:
    CriticalSection()
    {
        __asm__ volatile("mrs %0, PRIMASK" : "=r"(_disabled) : : "memory");
        __disable_irq();
    }
    ~CriticalSection()
    {
        if (!_disabled) {
            __enable_irq();
        }
    }
private:
    bool    _disabled;
};

#define CRITICAL_SECTION        CriticalSection _crit

uint32_t    Timer::_highWord = 0;
Timer       *Timer::_list = nullptr;
bool        Timer::_running = false;

Timer::Timer(Callback callback, void *arg) :
    _callback(callback),
    _arg(arg)
{
}

Timer::Time
Timer::time()
{
    CRITICAL_SECTION;

    uint32_t low;

    // Spin grabbing the low time word and checking for possible wrap.
    //
    // If the wrap flag is set after getting the low word, we don't know
    // whether we read it before or after the wrap, so we will have to
    // discard it and repeat the check.
    //
    for (;;) {
        // get the counter low word
        low = TMR_TMR32B1TC;

        // if the counter shows as wrapped, update the high word
        if ((TMR_TMR32B1IR & TMR_TMR32B1IR_MR0_MASK) == TMR_TMR32B1IR_MR0) {
            TMR_TMR32B1IR = TMR_TMR32B1IR_MR0;
            _highWord++;

        } else {
            break;
        }
    }

    return ((Time)_highWord << 32) + low;
}

void
Timer::run()
{
    while (_list != nullptr) {
        // is the head of the list ready to run?
        if (_list->_deadline <= time()) {

            Timer *tp = _list;
            _list = tp->_next;
            tp->_next = nullptr;

            tp->_deadline = 0;

            if (tp->_callback != nullptr) {
                tp->_callback(tp->_arg);
            }

        } else {
            // nothing to do for a while
            break;
        }
    }

    reschedule();
}

void
Timer::callAt(Time deadline)
{
    CRITICAL_SECTION;

    // cancel if we are still pending
    if (_deadline != 0) {
        cancel();
    }

    // save new deadline
    _deadline = deadline;

    // sort into the list of timers
    Timer **tpp = &_list;

    while (*tpp != nullptr) {
        if (_deadline < (*tpp)->_deadline) {
            _next = *tpp;
            break;
        }

        tpp = &((*tpp)->_next);
    }

    *tpp = this;

    // if it's at the head, we need to (re) set the match
    if (tpp == &_list) {
        reschedule();
    }
}

void
Timer::cancel()
{
    CRITICAL_SECTION;

    Timer **tpp = &_list;

    while (*tpp != nullptr) {
        if (*tpp == this) {
            *tpp = _next;
            _deadline = 0;

            if (tpp == &_list) {
                reschedule();
            }

            break;
        }

        tpp = &((*tpp)->_next);
    }
}

void __attribute__((used, constructor))
_timerInit()
{
    Timer::init();
}

void
Timer::init()
{
    // clock on
    SCB_SYSAHBCLKCTRL |= SCB_SYSAHBCLKCTRL_CT32B1;

    // reset timer
    TMR_TMR32B1TCR = TMR_TMR32B1TCR_COUNTERRESET_ENABLED;

    // prescale to microseconds
    TMR_TMR32B1PR = CFG_CPU_CCLK / 1000000;

    // schedule an interrupt on every wrap
    TMR_TMR32B1MR0 = 0xffffffff;
    TMR_TMR32B1MCR = TMR_TMR32B1MCR_MR0_INT_ENABLED;

    // start the timer
    TMR_TMR32B1TCR = TMR_TMR32B1TCR_COUNTERENABLE_ENABLED;

    // enable timer interrupts
    NVIC_EnableIRQ(TIMER_32_1_IRQn);

    _running = true;
    reschedule();
}

void
Timer::reschedule()
{
    // can't touch the timer yet...
    if (!_running) {
        return;
    }

    // disable match interrupt
    TMR_TMR32B1MCR &= ~TMR_TMR32B1MCR_MR1_INT_MASK;

    // clear any pending interrupt status
    TMR_TMR32B1IR = TMR_TMR32B1IR_MR1;

    // nothing to schedule, do nothing here
    if (_list == nullptr) {
        return;
    }

    Time now = time();
    Interval pop;

    // bound the interval to the next interrupt
    if (_list->_deadline < (now + _minInterval)) {
        pop = _minInterval;

    } else if (_list->_deadline > (now + _maxInterval)) {
        pop = _maxInterval;

    } else {
        pop = _list->_deadline - now;
    }

    // configure match 1 to pop at or before the deadline
    TMR_TMR32B1MR1 = TMR_TMR32B1TC + pop;
    TMR_TMR32B1MCR |= TMR_TMR32B1MCR_MR1_INT_ENABLED;
    NVIC_EnableIRQ(TIMER_32_1_IRQn);
}

extern "C" void TIMER_32_1_Handler(void);

void TIMER_32_1_Handler(void)
{
    // handle counter wrap interrupt
    if ((TMR_TMR32B1IR & TMR_TMR32B1IR_MR0_MASK) == TMR_TMR32B1IR_MR0) {
        // timer wrap routine will handle clearing interrupt flag
        (void)Timer::time();
    }

    // handle deadline expiration
    if ((TMR_TMR32B1IR & TMR_TMR32B1IR_MR1_MASK) == TMR_TMR32B1IR_MR1) {
        // run routine will handle clearing interrupt flag
        Timer::run();
    }
}

void
Ticker::tick(void *arg)
{
    reinterpret_cast<Ticker *>(arg)->_tick();
}

void
Ticker::_tick()
{
    _ticked = true;
    callAfter(_interval);
}

bool
Ticker::didTick() volatile
{
    if (_ticked) {
        _ticked = false;
        return true;
    }

    return false;
}
