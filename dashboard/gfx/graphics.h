/*
 * Function prototypes and interfaces.
 */

#pragma once

#include <stdint.h>

class Glyph;
class GlyphBitmap;
class GlyphAlert;
class Scene;
class Panel;

struct glyph_info;

/* an X/Y position */
struct Position {
    constexpr Position(unsigned px = 0, unsigned py = 0) : x(px), y(py) {}
    uint8_t x, y;

    Position        move(int dx, int dy) const { return Position(x + dx, y + dy); }
};

/* the dimensions of a rectangle */
struct Dimension {
    constexpr Dimension(unsigned dw = 0, unsigned dh = 0) : w(dw), h(dh) {}
    uint8_t w, h;

    Dimension       expand(unsigned by) { return Dimension(w + by, h + by); }
};

/* a rectangle at a specific location */
struct Region {
    constexpr Region(unsigned rx = 0, unsigned ry = 0, unsigned rw = 0, unsigned rh = 0) :
        p(rx, ry),
        d(rw, rh)
    {}
    constexpr Region(Position rp, Dimension rd) :
        p(rp),
        d(rd)
    {}

    Position p;
    Dimension d;

    Region          expand(unsigned by) { return Region(p.move(-(int)by, -(int)by), d.expand(2 * by)); }
};

/* a palette entry */
struct PaletteEntry {
public:
    static const unsigned depth = 5;

    constexpr PaletteEntry(uint8_t red, uint8_t green, uint8_t blue) :
        _raw(primaryBit(red,   0,  0) |
             primaryBit(green, 0,  1) |
             primaryBit(blue,  0,  2) |
             primaryBit(red,   1,  3) |
             primaryBit(green, 1,  4) |
             primaryBit(blue,  1,  5) |
             primaryBit(red,   2,  6) |
             primaryBit(green, 2,  7) |
             primaryBit(blue,  2,  8) |
             primaryBit(red,   3,  9) |
             primaryBit(green, 3, 10) |
             primaryBit(blue,  3, 11) |
             primaryBit(red,   4, 12) |
             primaryBit(green, 4, 13) |
             primaryBit(blue,  4, 14))
    {
    }
    constexpr PaletteEntry(uint16_t raw) : _raw(raw) {}

    constexpr uint16_t raw() const { return _raw; }

    unsigned        slice(unsigned shift) const { return (_raw >> (shift * 3)) & 0x7; }

private:
    const uint16_t  _raw;

    static constexpr unsigned       primaryBit(uint8_t primary, unsigned bit, unsigned shift)
    {
        return ((unsigned)primary & (1 << (8 - depth + bit))) ? (1 << shift) : 0;
    }
};

/* 16-colour palette */
typedef uint8_t                 Colour;
extern const PaletteEntry       palette[16];

/* palette indices */
#define Black           0
#define Red             1
#define DimRed          2
#define Blue            3
#define DimBlue         4
#define Green           5
#define DimGreen        6
#define Cyan            7
#define DimCyan         8
#define Magenta         9
#define DimMagenta      10
#define Yellow          11
#define DimYellow       12
#define Amber           13
#define DimWhite        14
#define White           15

/* compact framebuffer element for 16-colour palette */
struct Cell {
    typedef uint32_t Raw;

    constexpr static unsigned stride() { return sizeof(Raw) * 2; }

    Colour          get(unsigned index) const { return (_raw >> shift(index)) & 0xf; }
    void            set(unsigned index, unsigned val)
    {
        _raw = (_raw & mask(index)) | ((Raw)val << shift(index));
    }

    Raw             &raw() { return _raw; }

private:
    Raw             _raw;

    constexpr unsigned shift(unsigned index) { return index * 4; }
    constexpr Raw mask(unsigned index) { return ~((Raw)0xf << shift(index)); }
};

struct SubCell {
    SubCell(Cell &cell, unsigned index) : _cell(cell), _index(index) {}

    void                    set(Colour colour) { _cell.set(_index, colour); }
    Colour                  get() const { return _cell.get(_index); }
    const PaletteEntry      &getRGB() const { return palette[get()]; }

private:
    Cell            &_cell;
    const unsigned  _index;
};

/* a framebuffer */
template<unsigned ROWS, unsigned COLUMNS>
class FrameBufferTemplate
{
public:
    SubCell         subCell(unsigned address)
    {
        unsigned index = address % Cell::stride();
        return SubCell(_buffer[address / Cell::stride()], index);
    }

    SubCell         subCell(Position p)
    {
        unsigned address = p.y * COLUMNS + p.x;
        return subCell(address);
    }

    Cell            &cell(unsigned address)
    {
        return _buffer[address / Cell::stride()];
    }

    constexpr static unsigned rows() { return ROWS; }
    constexpr static unsigned columns() { return COLUMNS; }

    void            draw(Position p, Colour c)
    {
        subCell(p).set(c);
    }

    void            clear()
    {
        memset(_buffer, 0, sizeof(_buffer));
    }

private:
    Cell            _buffer[ROWS * COLUMNS / Cell::stride()];
};

// XXX should parameterise the framebuffer dimensions
typedef FrameBufferTemplate<32, 64>     FrameBuffer;

class Scene;

/* base class for a drawable thing */
class Glyph
{
public:
    Glyph(Scene *scene, Position p, Colour colour);

    virtual void    draw(Scene *in_scene);
    void            setColour(Colour colour) { _colour = colour; }

    Glyph           *next() { return _next; }
    void            push(Glyph *onto) { _next = onto; }

protected:
    Glyph           *_next;
    const Position  _p;
    Colour          _colour;

    void            drawBitmap(Scene *in_scene, const struct glyph_info &glyph, unsigned offset_x = 0, unsigned offset_y = 0);
    void            drawChar(Scene *in_scene, const uint8_t *font, uint8_t character, unsigned offset_x = 0, unsigned offset_y = 0);
};

/* a glyph that draws a bitmap */
class GlyphIcon : public Glyph
{
public:
    GlyphIcon(Scene *scene, Position p, const struct glyph_info &icon, Colour colour, const volatile bool &enable = ENABLED);


    virtual void    draw(Scene *in_scene) override;

protected:
    static const bool ENABLED;

    const volatile bool &_enable;
    const struct glyph_info &_icon;
    const Dimension _d;
};

/* a glyph that draws a number */
class GlyphNumber : public Glyph
{
public:
    GlyphNumber(Scene *scene, Position p, const uint8_t *font, unsigned digits, Colour colour, volatile unsigned &value);

    virtual void    draw(Scene *in_scene) override;

protected:
    const uint8_t   *_font;
    unsigned        _digits;
    volatile unsigned &_value;
};

/* a class that draws a number divided by 10 */
class GlyphNumberTenths : public GlyphNumber
{
public:
    GlyphNumberTenths(Scene *scene, Position p, const uint8_t *font, unsigned digits, Colour colour, volatile unsigned &value) :
        GlyphNumber(scene, p, font, digits, colour, value) {}
    virtual void draw(Scene *in_scene) override;
};

/* a glyph that owns a text field */
class GlyphText : public Glyph
{
public:
    typedef void    (* Generator)(GlyphText *owner);

    GlyphText(Scene *scene, Region r, const uint8_t *font, Colour colour, Generator generator);

    virtual void    draw(Scene *in_scene) override;
    void            emit(char c);
    void            emitf(const char *fmt, ...);
    void            emit_int(unsigned n, unsigned width);
    void            emit_hex(unsigned n, unsigned width);
    void            emit_string(const char *cp, unsigned width = 0);
    void            set_generator(Generator g) { _generator = g; }

private:
    const uint8_t   *_font;
    Dimension       _d;
    Generator       _generator;
    Position        _cursor;
    Scene           *_scene;
};

/* a glyph that draws a bargraph */
class GlyphBar : public Glyph
{
public:
    enum Orientation {
        O_HORIZONTAL,
        O_VERTICAL
    };

    GlyphBar(Scene *scene, Region r, Orientation o, unsigned min, unsigned max, Colour colour, volatile unsigned &value);

    virtual void    draw(Scene *in_scene) override;

private:
    Region          _r;
    Orientation     _o;
    volatile unsigned &_value;
    unsigned        _offset;
    unsigned        _range;
};

