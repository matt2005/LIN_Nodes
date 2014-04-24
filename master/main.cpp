
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "board.h"
#include "lin_protocol.h"

#include "display.h"
#include "master.h"
#include "menu.h"

Board           board;

int
main(void)
{
    uint8_t boardMode = Board::getMode();

    if (boardMode == 0) {
        // Board in master mode
        Master          master;

        // Enable interrupts; timers and LIN events will start.
        sei();

        // Initialise the display and set the backlight low to avoid
        // overheating the node power supply.
        Display disp;
        disp.setBacklight(10);
        disp.writeP(PSTR("Master Node OK"));
        Board::delay(2000);

        // Initialise the menu system
        Menu    menu(disp, master);

        // spin running the UI
        for (;;) {
            wdt_reset();
            menu.tick();
        }

    } else if (boardMode == 8) {
        // In board mode 8, we are an aux switch input block without a display.

        Slave slave(LIN::kNADAuxSwitches);

        // Enable interrupts; timers and LIN events will start.
        //
        sei();

        // spin tickling the watchdog
        for (;;) {
            wdt_reset();
        }

    } else {

        // board is in 'recovery' mode
        Board::panic(2);
    }
}

