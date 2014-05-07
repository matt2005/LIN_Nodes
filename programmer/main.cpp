
#include <avr/wdt.h>

#include "board.h"

#include "hd44780.h"

HD44780 lcd;
Display &gDisplay = lcd;

void
main(void)
{
    Board::init();
    lcd.init();

    gDisplay.printf(PSTR("test"));

    unsigned i = 0;

    for (;;) {
        wdt_reset();

        gDisplay.move(0, 1);
        gDisplay.printf(PSTR("count %u    "), i++);
    }

}

