/*
 * Copyright (c) 2012-2015, Mike Smith, <msmith@purgatory.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * o Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

///@file print.h

#pragma once

#include <stdarg.h>

#include <avr/pgmspace.h>

class Print
{
public:
    /// Simple printf
    ///
    /// Formats supported:
    ///
    /// %[width]s (char *)      - prints at most <width> characters from a progmem string.
    ///                           Pads with leading spaces.
    /// %p (void *)             - prints pointer with leading 0x
    /// %[width]x (unsigned)    - prints least significant <width> hex digits from value, width
    ///                           defaults to 4. Leading space padded with zeroes.
    /// %[width]u (unsigned)    - prints least significant <width> decimal digits from value,
    ///                           width defaults to 5. Leading space padded with spaces.
    /// %[width]t (char *, unsigned) - as for %s, but the string is looked up in a strtab.
    ///
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
    struct Position {
        constexpr Position(uint8_t at_x, uint8_t at_y) : x(at_x), y(at_y) {}
        uint8_t     x: 5;
        uint8_t     y: 3;
    };
    struct Dimension {
        constexpr Dimension(uint8_t w, uint8_t h) : width(w), height(h) {}
        uint8_t     width: 5;
        uint8_t     height: 3;
    };
    struct Region {
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
