
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
