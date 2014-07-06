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
    void            _writes(const char *s, uint8_t width);
};

class Display : public Print
{
public:
    Display(uint8_t columns, uint8_t rows) :
        _columns(columns),
        _rows(rows)
    {
    }

    virtual void    move(uint8_t, uint8_t y) = 0;
    virtual void    clear() = 0;

    uint8_t         columns() const { return _columns; }
    uint8_t         rows() const { return _rows; }

private:
    const uint8_t   _columns:5;
    const uint8_t   _rows:3;
};

extern Display &gDisplay;
