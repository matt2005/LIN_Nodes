// Board-specific functionality
//

#include <avr/power.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include "board.h"

namespace Board {

void
init()
{
    // set the prescaler for maximum speed
    clock_prescale_set(clock_div_1);

    // start the watchdog
    wdt_enable(WDTO_500MS);

    // LINCS starts tristated, keep it de-asserted as we turn it into an output
    pinLINCS.clear();
    pinLINCS.cfgOutput();

    // configure LIN data pins
    pinLINRX.cfgInputPullUp();
    pinLINTX.cfgInputPullUp();
}

uint8_t
getMode()
{
    uint8_t mode = 0;

    // configure Mode pins for reading
#ifdef pinMode0
    pinMode0.cfgInputPullUp();
#endif
#ifdef pinMode1
    pinMode1.cfgInputPullUp();
#endif
#ifdef pinMode2
    pinMode2.cfgInputPullUp();
#endif
#ifdef pinMode3
    pinMode3.cfgInputPullUp();
#endif

    // sample the mode pins
#ifdef pinMode0

    // allow inputs to settle
    _delay_ms(1);

    if (pinMode0.get())
        mode |= 1;
#endif
#ifdef pinMode1
    if (pinMode1.get())
        mode |= 2;
#endif
#ifdef pinMode2
    if (pinMode2.get())
        mode |= 4;
#endif
#ifdef pinMode3
    if (pinMode3.get())
        mode |= 8;
#endif

    return mode;
}

} // namespace Board
