
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "board.h"
#include "lin_protocol.h"
#include "mc33972.h"
#include "softserial.h"

#include "display.h"
#include "master.h"
#include "menu.h"

void
main(void)
{
    Board       board;

    // check for recovery mode before constructing anything else
    if (Board::getMode() != 0) {
        Board::panic(Board::kPanicRecovery);
    }

    // construct drivers that we need
    Master      master;         // XXX must be before switches to get !SS as an output 
    MC33972     switches;       // XXX should move LINCS somewhere else, use PA6 for SPI !SS

    // enable interrupts; timers and LIN events will start.
    sei();

    // check for an attached display, run setup mode if attached
    Display     disp;
    if (disp.probe()) {
        Menu        menu(disp, master);

        debug("Display found, entering setup mode");

        // run the menu state machine forever
        for (;;) {
            wdt_reset();
            menu.tick();
            switches.scan();
        }
    }

    // run the master logic forever
    for (;;) {
        wdt_reset();
        //menu.tick();
        switches.scan();
    }

}

