
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "board.h"
#include "slave.h"

Board           board;

void
main(void)
{
    uint8_t id = Board::getMode();

    if (Board::getMode() == 0) {
        // board is in 'recovery' mode
        Board::panic(2);
    }

    debug("relay slave %2u", id);
    debug("%3u free", Board::freemem());

    RelaySlave  slave(id);

    sei();

    for (;;) {
        wdt_reset();
    }
}

