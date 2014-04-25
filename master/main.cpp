
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "board.h"
#include "lin_protocol.h"

#include "display.h"
#include "master.h"
#include "menu.h"
#include "switches.h"

Board           board;

static void
main_master()
{
    Switches    switches(0);
    Master      master;
    Display     disp;
    Menu        menu(disp, master);

    // enable interrupts; timers and LIN events will start.
    sei();

    // Initialise the display and set the backlight low to avoid
    // overheating the node power supply.
    disp.setBacklight(10);
    disp.writeP(PSTR("Master Node OK"));
    Board::delay(2000);
    disp.setBacklight(0);

    // spin forever running the UI and polling switches
    for (;;) {
        wdt_reset();
        menu.tick();
        switches.scan();
    }
}

static void
main_slave()
{
    Switches    switches(0);
    Slave       slave(LIN::kNADAuxSwitches);

    // enable interrupts; timers and LIN events will start.
    sei();

    // spin tickling the watchdog and checking switches
    for (;;) {
        wdt_reset();
        switches.scan();
    }
}

int
main(void)
{
    switch (Board::getMode()) {
    case 0:
        main_master();
    case 8:
        main_slave();
    default:
        // board is in 'recovery' mode
        Board::panic(2);
    }
}

