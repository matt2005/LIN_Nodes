
#include <util/delay.h>
#include <avr/wdt.h>

#include "board.h"
#include "lin_protocol.h"

#include "display.h"

Display disp;

int
main(void)
{
    // basic board & system init
    Board::init();
    disp.init();

    // XXX for now, idle here
    for (;;) {
        disp.clear();
        disp.write("test");
        _delay_ms(200);
        wdt_reset();
    }
}

