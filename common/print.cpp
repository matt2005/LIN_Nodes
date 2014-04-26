
#include <avr/pgmspace.h>

#include "print.h"

void
Print::write(const char *s)
{
    _write(s, readChar);
}

void
Print::writeP(PGM_P s)
{
    _write(s, readCharP);
}

void
Print::write(uint8_t n)
{
    _write(n, 3);
}

void
Print::write(uint16_t n)
{
    _write(n, 5);
}

void
Print::_write(uint16_t n, uint8_t width)
{
    char buf[6];
    uint8_t pos = width;
    bool clear = false;

    buf[pos--] = 0;
    buf[pos] = '0';
    
    for (;;) {
        if (n > 0) {
            buf[pos] = '0' + n % 10;
            n /= 10;
        } else {
            if (clear) {
                buf[pos] = ' ';
            }
        }
        if (pos == 0) {
            break;
        }
        clear = true;
        pos--;
    } 
    _write(&buf[0], readChar);
}

uint8_t
Print::readChar(const char *p)
{
    return *p++;
}

uint8_t
Print::readCharP(const char *p)
{
    return pgm_read_byte(p++);
}
