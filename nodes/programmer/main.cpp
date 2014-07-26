/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

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

