#include <stdarg.h>

#include <avr/pgmspace.h>

#include "print.h"

void
Print::write(const char *s)
{
    uint8_t c;

    while ((c = *s++) != 0) {
        _write(c);
    }
}

void
Print::writeP(PGM_P s)
{
    uint8_t c;

    while ((c = pgm_read_byte(s++)) != 0) {
        _write(c);
    }
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
Print::printf(PGM_P fmt, ...)
{
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
    write(&buf[0]);
}
