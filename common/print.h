///@file print.h

#pragma once

#include <stdarg.h>

#include <avr/pgmspace.h>

class Print
{
public:
   
    void            write(const char *s);
    void            writeP(PGM_P s);
    void            write(uint8_t n);
    void            write(uint16_t n);

    void            printfP(PGM_P fmt, ...);

protected:
    typedef uint8_t (*Reader)(const char *p);

    virtual void    _write(uint8_t c) = 0;
    static uint8_t  readChar(const char *p);
    static uint8_t  readCharP(const char *p);

private:
    void            _write(uint16_t n, uint8_t width);
    void            _writex(uint16_t n, uint8_t width);
};
