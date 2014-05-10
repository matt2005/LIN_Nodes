
//
// LIN network programmer
//
// Master periodically emits kFIDConfigRequest headers. The programmer may
// respond with a ConfigFrame describing a configuration request to
// the network. Following a ConfigFrame the master will emit
// a kFIDConfigResponse header, and the addressed node (which may be the
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

HD44780 lcd;
Display &gDisplay = lcd;
ProgrammerSlave gSlave;

void
main(void)
{
    Board::init();
    lcd.init();
    Encoder::init();
    gSlave.init();

    sei();

    for (;;) {
        wdt_reset();

        Menu::tick();
    }

}

