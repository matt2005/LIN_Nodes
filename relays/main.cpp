
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "board.h"
#include "slave.h"


void
main(void)
{
    Board       board;
    uint8_t     id = Board::getMode();

    // check for recovery mode before constructing anything else
    if (Board::getMode() == 0) {
        Board::panic(Board::kPanicRecovery);
    }

    // construct the slave
    RelaySlave  slave(id);

    // enable interrupts
    sei();

    // run the relay logic forever
    for (;;) {
        wdt_reset();
        // XXX respond to frame updates, etc. here
    }
}

