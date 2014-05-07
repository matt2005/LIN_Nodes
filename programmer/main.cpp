
#include <avr/wdt.h>

#include "board.h"

#include "hd44780.h"

HD44780 Display;

void
main(void)
{
    Board::init();
    Display.init();

    Display.writeP(PSTR("test"));

    unsigned i = 0;

    for (;;) {
        wdt_reset();

        Display.move(0, 1);
        Display.printfP(PSTR("count %u    "), i++);
    }

}

