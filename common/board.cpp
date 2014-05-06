// Board-specific functionality
//

#include <avr/power.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay_basic.h>

#include "board.h"

extern "C" uint8_t _end;

namespace Board
{

#ifdef DEBUG
/// Debug serial port
///
Serial       debugPort;
#endif

void init()
{
    // set the prescaler for maximum speed
    clock_prescale_set(clock_div_1);

    // turn everything on
    power_all_enable();

    // start the watchdog
    wdt_enable(WDTO_500MS);

    // fill (most of) the stack with 0xff for sniffing purposes
    {
        volatile uint8_t *p = &_end;

        while (p < (uint8_t *)SP) {
            *p++ = 0xff;
        }
    }

    // do pin setup
#if defined(BOARD_POWER_V1)
    // power switch control pins
    pinOUT1.clear();
    pinOUT1.cfgOutput();
    pinOUT2.clear();
    pinOUT2.cfgOutput();
    pinOUT3.clear();
    pinOUT3.cfgOutput();
    pinOUT4.clear();
    pinOUT4.cfgOutput();

    // power switch status pins
    pinSTATUS1.cfgInputNoPull();
    pinSTATUS2.cfgInputNoPull();
    pinSTATUS3.cfgInputNoPull();
# ifndef DEBUG                      // debug() output on this pin
    pinSTATUS4.cfgInputNoPull();
# endif    
#endif

#if defined(BOARD_MASTER_V1)

    // LINCS/LINTX start tristated, configure for LIN use.
    pinLINCS.set();
    pinLINCS.cfgOutput();
    pinLINTX.cfgInputPullUp();
    pinLINRX.cfgInputPullUp();

    // SPI
    pinMOSI.cfgOutput();
    pinMISO.cfgInputNoPull();
    pinSCK.cfgOutput();
    pinCS.set();
    pinCS.cfgOutput();
#endif

}

void
panic(uint8_t code)
{
    debug("panic %3u @ %p", code, __builtin_return_address(0));

    // disable interrupts and wait for possible pending LIN transmit to 
    // complete
    cli();
    msDelay(10);

    // start in a state that doesn't risk powering us off
    pinLINTX.set();
    pinLINTX.cfgOutput();
    pinLINCS.clear();
    pinLINCS.cfgOutput();

    for (;;) {

        msDelay(1000);

        // blink the LIN CS LED with our error code
        for (uint8_t i = 0; i < code; i++) {
            wdt_reset();
            pinLINCS.set();
            msDelay(200);
            wdt_reset();
            pinLINCS.clear();
            msDelay(200);            
        }
    }
}

uint8_t
getMode()
{
    uint8_t mode = 0;

    // configure Mode pins for reading
    pinMode1.cfgInputPullUp();
    pinMode2.cfgInputPullUp();
    pinMode4.cfgInputPullUp();
    pinMode8.cfgInputPullUp();

    // sample the mode pins
    msDelay(10);                // allow inputs to settle

    if (!pinMode1.get())        // 1 bits are pulled low
        mode |= 1;
    if (!pinMode2.get())
        mode |= 2;
    if (!pinMode4.get())
        mode |= 4;
    if (!pinMode8.get())
        mode |= 8;

    return mode;
}

void
sleep()
{
    // by the time LINCS and LINTX are both driving 0, the board will power off
    pinLINCS.clear();
    pinLINTX.clear();
    pinLINCS.cfgOutput();
    pinLINTX.cfgOutput();
}

void
msDelay(uint16_t ms)
{
    while (ms > 0) {
        wdt_reset();
        usDelay(1000);   // XXX approximate
        ms--;
    }
    wdt_reset();
}

void
usDelay(uint16_t us)
{

    // _delay_loop_2 consumes 4 cycles per count, so convert microseconds to
    // units of 4 cycles
    us *= F_CPU / 4000000UL;

    _delay_loop_2(us);
}

uint16_t
freemem()
{
    // approximate free memory value
    volatile uint8_t *p = &_end;
    uint16_t mem = 0;

    while ((*p == 0xff) && (p < (uint8_t *)SP)) {
        mem++;
        p++;
    }

    return mem;
}

} // namespace Board
