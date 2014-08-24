/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

// Board-specific functionality
//

#include <avr/power.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
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
    // clear the reset status register and disable the watchdog (as it may be running)
    // XXX save reset cause for later use?
    MCUSR = 0;
    wdt_disable();

    // set the prescaler for maximum speed
    clock_prescale_set(clock_div_1);

    // turn everything on
    power_all_enable();

    // start the watchdog
    wdt_enable(WDTO_500MS);

    // fill (most of) the stack with 0xff for sniffing purposes
    volatile uint8_t *p = &_end;

    while (p < (uint8_t *)SP) {
        *p++ = 0xff;
    }
}

void
panic(uint8_t code)
{
    debug("panic %3u @ %p", code, __builtin_return_address(0));

    // disable interrupts and wait for possible pending LIN transmit to
    // complete
    cli();
    ms_delay(10);

    // start in a state that doesn't risk powering us off
    pinLINTX.set();
    pinLINTX.cfg_output();
    pinLINCS.clear();
    pinLINCS.cfg_output();

    for (;;) {

        ms_delay(1000);

        // blink the LIN CS LED with our error code
        for (uint8_t i = 0; i < code; i++) {
            wdt_reset();
            pinLINCS.set();
            ms_delay(200);
            wdt_reset();
            pinLINCS.clear();
            ms_delay(200);
        }
    }
}

uint8_t
get_mode()
{
    uint8_t mode = 0;

    // configure Mode pins for reading
#ifdef pinMode1
    pinMode1.cfg_input_pullup();
#ifdef pinMode2
    pinMode2.cfg_input_pullup();
#ifdef pinMode4
    pinMode4.cfg_input_pullup();
#ifdef pinMode8
    pinMode8.cfg_input_pullup();
#endif
#endif
#endif
#endif


    // sample the mode pins
    ms_delay(10);                // allow inputs to settle

#ifdef pinMode1

    if (!pinMode1.get())        // 1 bits are pulled low
        mode |= 1;

#ifdef pinMode2

    if (!pinMode2.get())
        mode |= 2;

#ifdef pinMode4

    if (!pinMode4.get())
        mode |= 4;

#ifdef pinMode8

    if (!pinMode8.get())
        mode |= 8;

#endif
#endif
#endif
#endif

    return mode;
}

void
sleep()
{
    // Put the board to sleep.
    //
    for (;;) {
        LINCR = LSWRES;         // stop the LIN block
        ms_delay(10);           // wait for any frame to complete
        pinLINTX.set();         // make sure TX is 1
        pinLINCS.clear();       // make sure CS is 0
        pinLINCS.cfg_output();  // make sure CS is an output (will be low now)
        pinLINTX.cfg_output();  // make sure TX is an output (will be high now)
        pinLINTX.clear();       // drive TX low to turn off power
        ms_delay(10);
        // if we failed to go to sleep, the watchdog will pull us
        // back out via the reset path
    }
}

void
ms_delay(uint16_t ms)
{
    while (ms > 0) {
        wdt_reset();
        us_delay(1000);   // XXX approximate
        ms--;
    }

    wdt_reset();
}

void
us_delay(uint16_t us)
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

void
enter_bootloader(uint8_t nad, uint8_t function)
{
    // make sure the watchdog doesn't catch us while we're updating the EEPROM
    wdt_reset();

    // write magic to the EEPROM to cause us to wait in the bootloader
    eeprom_update_byte((uint8_t *)E2END - 3, nad);
    eeprom_update_byte((uint8_t *)E2END - 2, function);
    eeprom_update_word((uint16_t *)(E2END - 1), 0x4f42);

    // and wait for the watchdog to reset us
    for (;;)
        ;
}

} // namespace Board
