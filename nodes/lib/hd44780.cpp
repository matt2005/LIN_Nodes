
#include <avr/wdt.h>

#include "board.h"

#include "hd44780.h"

void
HD44780::init()
{
    // configure LCD interface pins
    portLCD.DDR = 0x7f;
    portLCD.set(0);

    // wait to allow display to boot
    Board::ms_delay(100);

    // configure display for 4-bit mode
    // http://web.alfredstate.edu/weimandn/index.html
    send_nibble(0x3);
    Board::ms_delay(10);

    send_nibble(0x3);
    Board::us_delay(200);

    send_nibble(0x3);
    Board::us_delay(200);

    send_nibble(0x2);
    Board::us_delay(80);

    // configure for 2 lines
    send_cmd(0x28);
    Board::us_delay(80);

    // display off (?)
    send_cmd(0x08);
    Board::us_delay(80);

    // clear display & home cursor
    send_cmd(0x01);
    Board::ms_delay(4);

    // set cursor direction
    send_cmd(0x06);
    Board::us_delay(80);

    // turn on display
    send_cmd(0x0c);
    Board::us_delay(80);
}

void
HD44780::clear()
{
    // clear display & home cursor
    send_cmd(0x01);
    Board::ms_delay(4);
}

void
HD44780::move(Position p)
{
    uint8_t cmd = 0x80;

    if (p.y & 1) {
        cmd |= 0x40;
    }
    if (p.y & 2) {
        cmd |= 0x14;
    }

    send_cmd(cmd + p.x);
}

void
HD44780::write(uint8_t c)
{
    send_nibble((c >> 4)  | bitDnC);
    send_nibble((c & 0xf) | bitDnC);
    Board::us_delay(80);
}

void
HD44780::send_nibble(uint8_t val)
{
    portLCD.set(val);
    portLCD.set(val | bitE);
    Board::us_delay(10);
    portLCD.set(val);
    Board::us_delay(10);
}

void
HD44780::send_cmd(uint8_t cmd)
{
    send_nibble(cmd >> 4);
    send_nibble(cmd & 0xf);
    Board::us_delay(80);
}
