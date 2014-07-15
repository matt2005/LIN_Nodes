
//
// LIN network programmer
//
// Master periodically emits kFrameIDConfigRequest headers. The programmer may
// respond with a ConfigFrame describing a configuration request to
// the network. Following a ConfigFrame the master will emit
// a kFrameIDConfigResponse header, and the addressed node (which may be the
// master) may respond with a ConfigFrame. Not all programmer requests expect
// a response.
//

#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "board.h"

#include "hd44780.h"
#include "menu.h"
#include "encoder.h"
#include "slave.h"

HD44780 lcd(Display::Dimension(20, 4));
Display &gDisplay = lcd;
ProgrammerSlave gSlave;

void
main(void)
{
    Board::init();

    if (Board::get_mode() != 0) {
        Board::panic(Board::kPanicCodeRecovery);
    }

    lcd.init();
    Encoder::init();
    gSlave.init();

    debug("programmer starting");

    sei();

    for (;;) {
        wdt_reset();

        Menu::tick();
    }

}

