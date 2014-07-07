
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
    Board::msDelay(100);

    // configure display for 4-bit mode
    // http://web.alfredstate.edu/weimandn/index.html
    sendNibble(0x3);
    Board::msDelay(10);

    sendNibble(0x3);
    Board::usDelay(200);

    sendNibble(0x3);
    Board::usDelay(200);

    sendNibble(0x2);
    Board::usDelay(80);

    // configure for 2 lines
    sendCmd(0x28);
    Board::usDelay(80);

    // display off (?)
    sendCmd(0x08);
    Board::usDelay(80);

    // clear display & home cursor
    sendCmd(0x01);
    Board::msDelay(4);

    // set cursor direction
    sendCmd(0x06);
    Board::usDelay(80);

    // turn on display
    sendCmd(0x0c);
    Board::usDelay(80);
}

void
HD44780::clear()
{
    // clear display & home cursor
    sendCmd(0x01);
    Board::msDelay(4);
}

void
HD44780::move(uint8_t x, uint8_t y)
{
    uint8_t cmd = 0x80;

    if (y & 1) {
        cmd |= 0x40;
    }
    if (y & 2) {
        cmd |= 0x14;
    }

    sendCmd(cmd + x);
}

void
HD44780::_write(uint8_t c)
{
    sendData(c);
}

void
HD44780::sendNibble(uint8_t val)
{
    portLCD.set(val);
    portLCD.set(val | bitE);
    Board::usDelay(10);
    portLCD.set(val);
    Board::usDelay(10);
}

void
HD44780::sendCmd(uint8_t cmd)
{
    sendNibble(cmd >> 4);
    sendNibble(cmd & 0xf);
    Board::usDelay(80);
}

void
HD44780::sendData(uint8_t val)
{
    sendNibble((val >> 4)  | bitDnC);
    sendNibble((val & 0xf) | bitDnC);
    Board::usDelay(80);
}
