// Board-specific functionality
//

#include <avr/power.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include "board.h"

extern "C" void panic(uint8_t code) { Board::panic(code); }

namespace Board {

void
init()
{
    // set the prescaler for maximum speed
    clock_prescale_set(clock_div_1);

    // turn everything on
    power_all_enable();

    // start the watchdog
    wdt_enable(WDTO_500MS);

    // configure LIN data pins
    pinLINRX.cfgInputPullUp();
    pinLINTX.cfgInputPullUp();

    // LINCS starts tristated, keep it de-asserted as we turn it into an output
    pinLINCS.clear();
    pinLINCS.cfgOutput();
}

void
panic(uint8_t code)
{
    for (;;) {
        // start in a state that doesn't risk powering us off
        pinLINTX.set();
        pinLINTX.cfgOutput();
        pinLINCS.cfgOutput();
        pinLINCS.clear();

        _delay_ms(250);

        // blink the LIN CS LED with our error code
        for (uint8_t i = 0; i < code; i++) {
            wdt_reset();
            pinLINCS.set();
            _delay_ms(200);
            wdt_reset();
            pinLINCS.clear();
            _delay_ms(200);            
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
    _delay_ms(10);              // allow inputs to settle

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

} // namespace Board
