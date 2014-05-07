
#include "board.h"

#include "hd44780.h"

void
HD44780::init()
{
    // configure LCD interface pins
    portLCD.DDR = 0x7f;

    // wait for 15ms to allow display to boot
    Board::msDelay(20);

    // configure display for 4-bit mode
    sendNibble(0x3);
    sendNibble(0x3);
    sendNibble(0x3);
    sendNibble(0x2);

    // configure for 2 lines
    sendCmd(0x28);

    // hide cursor
    sendCmd(0x08);

    clear();

    // set cursor direction
    sendCmd(0x06);

    // turn on display
    sendCmd(0x0c);    
}

void
HD44780::clear()
{
    // clear display & home cursor
    sendCmd(0x01);
}

void
HD44780::move(uint8_t x, uint8_t y)
{
//    sendCmd(0x80 | (x + (y ? 0x40 : 0)));
    sendCmd(0xc0);
}

void
HD44780::_write(uint8_t c)
{
    sendData(c);
}

void
HD44780::sendNibble(uint8_t val)
{
    portLCD.set(0);
    portLCD.set(val);
    portLCD.set(val | bitE);
    portLCD.set(val | bitE);
    portLCD.set(val | bitE);
    portLCD.set(val | bitE);
    portLCD.set(val);
    if (val & bitDnC) {
        Board::usDelay(40);
    } else {
        Board::msDelay(2);
    }
}

void
HD44780::sendCmd(uint8_t cmd)
{
    sendNibble(cmd >> 4);
    sendNibble(cmd & 0xf);
}

void
HD44780::sendData(uint8_t val)
{
    sendNibble((val >> 4)  | bitDnC);
    sendNibble((val & 0xf) | bitDnC);
}
