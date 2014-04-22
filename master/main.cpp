
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
Display         disp;
Menu            menu(disp);

Master          gMaster;
Event           controlsRequest(0, LIN::kFIDControls);
Event           masterRequest(1, LIN::kFIDMasterRequest);
Event           slaveResponse(1, LIN::kFIDSlaveResponse);


// for timer testing
//void blink(void *arg) { pinLINCS.toggle(); }

int
main(void)
{
    // Dim display backlight to reduce current & keep node PSU from
    // overheating when the display is powered from it.
    //
    disp.setBacklight(10);

    // Check board mode for 'recovery' mode.
    // XXX it would be good to be able to do this earlier.
    //
    if (Board::getMode() != 0) {
        Board::panic(2);
    }
    
    // Enable interrupts; timers and LIN events will start.
    //
    sei();

    // Post a message to the display
    //
    disp.writeP(PSTR("Master Node OK"));
    Board::delay(2000);

    // spin running the UI
    for (;;) {
        wdt_reset();
        menu.tick();
    }
}

