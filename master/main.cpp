
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
Master          gMaster;

int
main(void)
{
    uint8_t boardMode = Board::getMode();

    if (boardMode == 0) {
        // Board in master mode with an attached display

        // Create master-mode events
        Event   controlsRequest(0, LIN::kFIDControls);
        Event   masterRequest(1, LIN::kFIDMasterRequest);
        Event   slaveResponse(1, LIN::kFIDSlaveResponse);

        // Enable interrupts; timers and LIN events will start.
        //
        sei();

        // Create display and menu system
        Display disp;
        Menu    menu(disp);

        // set the backlight low to avoid overheating the node power supply
        disp.setBacklight(10);

        // post a message to the display
        disp.writeP(PSTR("Master Node OK"));
        Board::delay(2000);

        // spin running the UI
        for (;;) {
            wdt_reset();
            menu.tick();
        }

    } else if (boardMode == 8) {
        // In board mode 8, we are an aux switch input block without a display.

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

