/*
 * Glyphs.
 */

#include "graphics.h"
#include "scene.h"
#include "glyphs.h"

Glyph::Glyph(Scene &scene, Position p, Colour colour) :
    _next(nullptr),
    _p(p),
    _colour(colour)
{
    scene.addGlyph(this);
}

void
Glyph::draw(Scene *in_scene)
{
}

void
Glyph::drawBitmap(Scene *in_scene,
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
Glyph::drawChar(Scene *in_scene,
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

const bool GlyphIcon::ENABLED = true;

GlyphIcon::GlyphIcon(Scene &scene,
                     Position p,
                     const struct glyph_info &icon,
                     Colour colour,
                     const volatile bool &enable) :
    Glyph(scene, p, colour),
    _enable(enable),
    _icon(icon),
    _d(Dimension(GLYPH_WIDTH(icon.info), GLYPH_HEIGHT(icon.info)))
{
}

void
GlyphIcon::draw(Scene *in_scene)
{
    if (!_enable)
        return;

    Glyph::drawBitmap(in_scene, _icon);
}

GlyphNumber::GlyphNumber(Scene &scene,
                         Position p,
                         const uint8_t *font,
                         unsigned digits,
                         Colour colour,
                         volatile unsigned &value) :
    Glyph(scene, p, colour),
    _font(font),
    _digits(digits),
    _value(value)
{
}

void
GlyphNumber::draw(Scene *in_scene)
{
    unsigned w = _font[0];
    unsigned offset_x = _digits * w;
    unsigned v = _value;
    bool lsd = true;

    do {
        offset_x -= w;
        unsigned index = v % 10;

        if ((v > 0) | lsd) {
            Glyph::drawChar(in_scene, _font, '0' + index, offset_x);

        } else {
            Glyph::drawChar(in_scene, _font, ' ' + index, offset_x);
        }

        v /= 10;
        lsd = false;

    } while (offset_x > 0);
}

GlyphText::GlyphText(Scene &scene,
                     Position p,
                     const uint8_t *font,
                     unsigned width,
                     Colour colour,
                     const char *&text) :
    Glyph(scene, p, colour),
    _font(font),
    _width(width),
    _text(text)
{
}

void
GlyphText::draw(Scene *in_scene)
{
    if (_text == nullptr)
        return;

    unsigned w = _font[0];
    unsigned offset_x = 0;

    for (unsigned pos = 0; pos < _width; offset_x += w) {

        if (_text[pos] != '\0') {
            Glyph::drawChar(in_scene, _font, _text[pos++], offset_x);

        } else {
            Glyph::drawChar(in_scene, _font, ' ', offset_x);
        }
    }
}

GlyphBar::GlyphBar(Scene &scene,
                   Region r,
                   Orientation o,
                   unsigned min,
                   unsigned max,
                   Colour colour,
                   volatile unsigned &value) :
    Glyph(scene, r.p, colour),
    _r(r),
    _o(o),
    _value(value),
    _offset(min),
    _range(max - min)
{
}

void
GlyphBar::draw(Scene *in_scene)
{
    unsigned max_bars = (_o == O_HORIZONTAL) ? _r.d.w : _r.d.h;
    unsigned blank_bars;
    Region	r = _r;

    if (_value <= _offset) {
        blank_bars = max_bars;

    } else {
        _value -= _offset;

        if (_value >= _range) {
            blank_bars = 0;

        } else {
            /* XXX rounding */
            blank_bars = max_bars - ((_value * max_bars) / _range);
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
