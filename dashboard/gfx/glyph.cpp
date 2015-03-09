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

/*
 * Glyphs.
 */

#include <stdarg.h>

#include "graphics.h"
#include "scene.h"
#include "glyphs.h"

Glyph::Glyph(Scene *scene, Position p, Colour colour) :
    _next(nullptr),
    _p(p),
    _colour(colour)
{
    if (scene != nullptr)
        scene->add_glyph(this);
}

void
Glyph::draw(const Scene *in_scene)
{
}

void
Glyph::draw_bitmap(const Scene *in_scene,
                   const struct glyph_info &glyph,
                   unsigned offset_x,
                   unsigned offset_y)
{
    Position origin = _p.move(offset_x, offset_y);
    const uint16_t *dp = &glyph.data[0];
    unsigned bits = 0;
    unsigned bc = 0;

    unsigned tc = origin.x + GLYPH_WIDTH(glyph.info);
    unsigned tr = origin.y + GLYPH_HEIGHT(glyph.info);

    for (unsigned r = origin.y; r < tr; r++) {
        for (unsigned c = origin.x; c < tc; c++) {
            if (bc == 0) {
                bits = *dp++;
                bc = 8;
            }

            /* if alpha is set */
            if (bits & 2) {
                /* overwrite whatever's there with our bitmap data */
                in_scene->draw(Position(c, r), (bits & 1) ? _colour : Black);
            }

            bc--;
            bits >>= 2;
        }

        /* force each row to consume a whole word - makes editing easier */
        bc = 0;
    }
}

void
Glyph::draw_character(const Scene *in_scene,
                      const uint8_t *font,
                      uint8_t character,
                      unsigned offset_x,
                      unsigned offset_y)
{
    unsigned f_width = font[0];
    unsigned f_bpr = (f_width + 7) / 8;
    unsigned f_height = font[1];
    unsigned f_nchar = font[2];
    unsigned f_index;

    for (f_index = 0; f_index < f_nchar; f_index++) {
        if (font[3 + f_index] == character)
            break;
    }

    /* not present in the font */
    if (f_index >= f_nchar)
        return;

    Position origin = _p.move(offset_x, offset_y);
    const uint8_t *f_data = &font[3 + f_nchar + (f_index * f_bpr * f_height)];

    unsigned tr = origin.y + f_height;

    for (unsigned r = origin.y; r < tr; r++) {
        unsigned c = origin.x;
        unsigned resid = f_width;

        do {
            uint8_t bits = *f_data++;

            for (int b = 7; (b >= 0) && (resid > 0); b--, resid--, c++) {
                /* overwrite whatever's there with our bitmap data */
                in_scene->draw(Position(c, r), (bits & (1 << b)) ? _colour : Black);
            }
        } while (resid > 0);
    }
}

void
GlyphIcon::draw(const Scene *in_scene)
{
    if (!_enable)
        return;

    Glyph::draw_bitmap(in_scene, _icon);
}

void
GlyphNumber::draw(const Scene *in_scene)
{
    unsigned w = _font[0];
    unsigned offset_x = (_digits - 1) * w;
    unsigned v = _value;

    if (!_value.is_valid()) {
        Glyph::draw_character(in_scene, _font, '-', offset_x);

    } else {
        bool lsd = true;

        do {
            unsigned index = v % 10;

            if ((v > 0) || lsd) {
                Glyph::draw_character(in_scene, _font, '0' + index, offset_x);

            } else {
                Glyph::draw_character(in_scene, _font, ' ' + index, offset_x);
            }

            v /= 10;
            lsd = false;
            offset_x -= w;

        } while (offset_x > 0);
    }
}

void
GlyphNumberTenths::draw(const Scene *in_scene)
{
    unsigned w = _font[0];
    unsigned offset_x = (_digits - 1) * w + 2;
    unsigned v = _value;


    if (!_value.is_valid()) {
        Glyph::draw_character(in_scene, _font, '-', offset_x);

    } else {
        bool frac = true;
        bool lsd = false;

        do {
            unsigned index = v % 10;

            if ((v > 0) || lsd || frac) {
                Glyph::draw_character(in_scene, _font, '0' + index, offset_x);

            } else {
                Glyph::draw_character(in_scene, _font, ' ' + index, offset_x);
            }

            v /= 10;

            if (frac) {
                frac = false;
                lsd = true;

                /* decimal point - adjustment for descender is font-specific... */
                in_scene->draw(Position(_p.x + offset_x - 2, _p.y + _font[1] - 2), _colour);
                offset_x -= 2;

            } else {
                lsd = false;
            }

            offset_x -= w;

        } while (offset_x > 0);
    }
}

void
GlyphText::draw(const Scene *in_scene)
{
    in_scene->fill(Region(_p, _d), Black);

    _cursor.x = 0;
    _cursor.y = 0;
    _scene = in_scene;

    if (_generator != nullptr) {
        _generator(this);
    }
}

void
GlyphText::emit(char c)
{
    if (c == '\n') {
        _cursor.y += _font[1];
        _cursor.x = 0;
        return;
    }

    // XXX will spill right/bottom if region is not font-size multiple
    if ((_cursor.x >= _d.w) ||
        (_cursor.y >= _d.h)) {
        return;
    }

    draw_character(_scene, _font, c, _cursor.x, _cursor.y);
    _cursor.x += _font[0];
}

void
GlyphText::emit_int(unsigned n, unsigned width)
{
    char buf[width];
    unsigned pos = width - 1;
    bool clear = false;

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

    while (pos < width) {
        emit(buf[pos++]);
    }
}

void
GlyphText::emit_hex(unsigned n, unsigned width)
{
    uint8_t shift = 4 * (width - 1);

    for (;;) {
        uint8_t d = (n >> shift) & 0xf;

        if (d <= 9) {
            emit('0' + d);

        } else {
            emit('A' + d - 10);
        }

        if (shift == 0) {
            break;
        }

        shift -= 4;;
    }
}

void
GlyphText::emit_string(const char *s, unsigned width)
{
    if (width == 0) {
        width = 1000;
    }

    while ((*s != 0) && (width-- > 0)) {
        emit(*s++);
    }
}

void
GlyphText::emitf(const char *fmt, ...)
{
    va_list ap;
    uint8_t c;
    Colour ocolour = _colour;

    va_start(ap, fmt);

    while ((c = *fmt++) != 0) {

        // non-format characters
        if (c != '%') {
            emit(c);
            continue;
        }

        // initalise formatter
        uint8_t w = 0;

nextfmt:
        c = *fmt++;

        switch (c) {
        case '\0':
            goto out;

        case '1'...'9':
            w = (10 * w) + (c - '0');
            goto nextfmt;

        case 's':
            emit_string(va_arg(ap, const char *), w);
            break;

        case 'p':
            emit('0');
            emit('X');

        /* FALLTHROUGH */
        case 'u':
            emit_int(va_arg(ap, unsigned), w ? w : 5);
            break;

        case 'x':
            emit_hex(va_arg(ap, unsigned), w ? w : 8);
            break;

        case 'C':
            w = va_arg(ap, unsigned);

            if (w <= White) {
                _colour = w;
            }

            break;

        default:
            break;
        }
    }

out:
    _colour = ocolour;
}

void
GlyphBar::draw(const Scene *in_scene)
{
    unsigned max_bars = (_o == O_HORIZONTAL) ? _r.d.w : _r.d.h;
    unsigned blank_bars;
    Region	r = _r;
    unsigned v = _value;

    if (v <= _offset) {
        blank_bars = max_bars;

    } else {
        v -= _offset;

        if (v >= _range) {
            blank_bars = 0;

        } else {
            /* XXX rounding */
            blank_bars = max_bars - ((v * max_bars) / _range);
        }
    }

    if (_o == O_HORIZONTAL) {
        r.d.w -= blank_bars;

    } else {
        r.p.y += blank_bars;
        r.d.h -= blank_bars;
    }

    // XXX need to clear the blank area
    in_scene->fill(r, _colour);
}
