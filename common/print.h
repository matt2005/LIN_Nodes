///@file print.h

/// XXX see https://github.com/johnmccombs/arduino-libraries/blob/master/Flash/Flash.h


#pragma once

#include <stdarg.h>

#include <avr/pgmspace.h>

class Print
{
public:
    void            printf(PGM_P fmt, ...);
    void            putc(char c) { _write(c); }

protected:
    virtual void    _write(uint8_t c) = 0;

private:
    void            _write(uint16_t n, uint8_t width);
    void            _writex(uint16_t n, uint8_t width);
};

class Display : public Print
{
public:
    virtual void    move(uint8_t, uint8_t y) = 0;
    virtual void    clear() = 0;
};

extern Display &gDisplay;
