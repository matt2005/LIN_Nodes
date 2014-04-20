
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "board.h"
#include "lin_protocol.h"

#include "display.h"
#include "master.h"
#include "menu.h"

Master          master;
Event           controlsRequest(0, LIN::kFIDControls);
Event           masterRequest(1, LIN::kFIDMasterRequest);
Event           slaveResponse(1, LIN::kFIDSlaveResponse);

Display         disp;
Menu            menu(disp);

// for timer testing
//void blink(void *arg) { pinLINCS.toggle(); }

int
main(void)
{
    // basic board & system init
    Board::init();
    if (Board::getMode() != 0) {
        // stop here as we are in 'recovery' mode
        Board::panic(2);
    }
    Master::init();
    Timer::init();
    sei();

    // display init
    if (!disp.init()) {
        Board::panic(3);
    }

    // dim backlight to reduce current & keep node PSU from
    // overheating when the display is powered from it
    disp.setBacklight(10);

    // sign on
    disp.writeP(PSTR("Master Node OK"));
    Board::delay(2000);

    // spin running the UI
    for (;;) {
        wdt_reset();
        menu.tick();
    }
}

