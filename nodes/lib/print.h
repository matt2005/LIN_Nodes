///@file print.h

/// XXX see https://github.com/johnmccombs/arduino-libraries/blob/master/Flash/Flash.h


#pragma once

#include <stdarg.h>

#include <avr/pgmspace.h>

class Print
{
public:
    void            printf(PGM_P fmt, ...);
    void            putc(char c) { write(c); }

protected:
    virtual void    write(uint8_t c) = 0;

private:
    void            write(uint16_t n, uint8_t width);
    void            writex(uint16_t n, uint8_t width);
    void            write(const char *s, uint8_t width);
};

class Display : public Print
{
public:
    struct Position
    {
        constexpr Position(uint8_t at_x, uint8_t at_y) : x(at_x), y(at_y) {}
        uint8_t     x:5;
        uint8_t     y:3;
    };
    struct Dimension
    {
        constexpr Dimension(uint8_t w, uint8_t h) : width(w), height(h) {}
        uint8_t     width:5;
        uint8_t     height:3;
    };
    struct Region
    {
        constexpr Region(Position at, Dimension size) : p(at), d(size) {}
        constexpr Region(uint8_t x = 0, uint8_t y = 0, uint8_t width = 0, uint8_t height = 0) : p(x, y), d(width, height) {}
        Position    p;
        Dimension   d;
    };

    Display(Dimension size) :
        _size(size)
    {
    }

    virtual void    move(Position p) = 0;
    virtual void    clear() = 0;

    void            move(uint8_t x, uint8_t y) { move(Position(x, y)); }
    void            clear(Region region)
    {
        for (uint8_t r = 0; r < region.d.height; r++) {
            for (uint8_t c = 0; c < region.d.width; c++) {
                move(Position(region.p.x + c, region.p.y + r));
                write(' ');
            }
        }
    }

    uint8_t         columns() const { return _size.width; }
    uint8_t         rows() const { return _size.height; }

private:
    Dimension       _size;
};

extern Display &gDisplay;
