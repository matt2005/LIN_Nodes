
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "board.h"

void
main(void)
{
    Board::init();

    Board::panic(Board::kPanicCodeRecovery);

    debug("USB Link starting");

    sei();

    for (;;) {
        wdt_reset();
    }
}

