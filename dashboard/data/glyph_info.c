/*
 * Glyph library.
 */

#include "glyphs.h"

#define X   0x3 // foreground
#define _   0x2 // background

// construct a byte from 4 pixels
#define p(_a, _b, _c, _d, _e, _f, _g, _h)   _a | (_b << 2) | (_c << 4) | (_d << 6) | \
    (_e << 8) | (_f << 10) | (_g << 12) | (_h << 14)

const struct glyph_info g_barseg = {
    GLYPH_MONO(3, 1),
    {
        p(X, 0, 0, 0, 0, 0, 0, 0),
        p(X, 0, 0, 0, 0, 0, 0, 0),
        p(X, 0, 0, 0, 0, 0, 0, 0),
    }
};

const struct glyph_info g_denied = {
    GLYPH_MONO(8, 8),
    {
        p(_, _, X, X, X, X, _, _),
        p(_, X, _, _, _, _, X, _),
        p(X, _, _, _, _, X, _, X),
        p(X, _, _, _, X, _, _, X),
        p(X, _, _, X, _, _, _, X),
        p(X, _, X, _, _, _, _, X),
        p(_, X, _, _, _, _, X, _),
        p(_, _, X, X, X, X, _, _),
    }
};

const struct glyph_info g_x = {
    GLYPH_MONO(7, 7),
    {
        p(_, X, _, _, _, X, _, 0),
        p(X, X, X, _, X, X, X, 0),
        p(_, X, X, X, X, X, _, 0),
        p(_, _, X, X, X, _, _, 0),
        p(_, X, X, X, X, X, _, 0),
        p(X, X, X, _, X, X, X, 0),
        p(_, X, _, _, _, X, _, 0),
    }
};

const struct glyph_info g_tick = {
    GLYPH_MONO(6, 7),
    {
        p(_, _, _, _, _, X, _, 0),
        p(_, _, _, _, X, X, X, 0),
        p(_, X, _, X, X, X, _, 0),
        p(X, X, X, X, X, _, _, 0),
        p(_, X, X, X, _, _, _, 0),
        p(_, _, X, _, _, _, _, 0),
    }
};

const struct glyph_info g_battery = {
    GLYPH_MONO(6, 4),
    {
        p(0, X, X, 0, 0, 0, 0, 0),
        p(X, X, X, X, 0, 0, 0, 0),
        p(X, 0, 0, X, 0, 0, 0, 0),
        p(X, 0, 0, X, 0, 0, 0, 0),
        p(X, 0, 0, X, 0, 0, 0, 0),
        p(X, X, X, X, 0, 0, 0, 0),
    }
};

const struct glyph_info g_oildrop = {
    GLYPH_MONO(7, 6),
    {
        p(0, 0, X, 0, 0, 0, 0, 0),
        p(0, 0, X, 0, 0, 0, 0, 0),
        p(0, X, X, X, 0, 0, 0, 0),
        p(0, X, X, X, 0, 0, 0, 0),
        p(X, 0, X, X, X, 0, 0, 0),
        p(X, 0, X, X, X, 0, 0, 0),
        p(0, X, X, X, 0, 0, 0, 0),
    }
};

const struct glyph_info g_exclamation = {
    GLYPH_MONO(8, 4),
    {
        p(_, X, X, _, 0, 0, 0, 0),
        p(X, X, X, X, 0, 0, 0, 0),
        p(X, X, X, X, 0, 0, 0, 0),
        p(_, X, X, _, 0, 0, 0, 0),
        p(_, X, X, _, 0, 0, 0, 0),
        p(_, _, _, _, 0, 0, 0, 0),
        p(_, X, X, _, 0, 0, 0, 0),
        p(_, X, X, _, 0, 0, 0, 0),
    }
};

const struct glyph_info g_wrench = {
    GLYPH_MONO(8, 8),
    {
        p(_, _, X, X, _, _, _, _),
        p(_, _, _, X, X, _, _, _),
        p(X, _, X, X, X, _, _, _),
        p(X, X, X, X, X, X, _, _),
        p(_, X, X, X, _, X, X, _),
        p(_, _, _, X, X, _, X, X),
        p(_, _, _, _, X, X, X, X),
        p(_, _, _, _, _, X, X, _)
    }
};

const struct glyph_info g_barrel = {
    GLYPH_MONO(5, 5),
    {
        p(X, X, X, X, X, 0, 0, 0),
        p(0, X, 0, X, 0, 0, 0, 0),
        p(X, X, X, X, X, 0, 0, 0),
        p(0, X, 0, X, 0, 0, 0, 0),
        p(X, X, X, X, X, 0, 0, 0),
    }
};

const struct glyph_info g_water = {
    GLYPH_MONO(7, 4),
    {
        p(0, X, X, 0, 0, 0, 0, 0),
        p(0, X, X, X, 0, 0, 0, 0),
        p(0, X, X, 0, 0, 0, 0, 0),
        p(0, X, X, X, 0, 0, 0, 0),
        p(0, X, X, 0, 0, 0, 0, 0),
        p(X, X, X, X, 0, 0, 0, 0),
        p(0, X, X, 0, 0, 0, 0, 0),
    }
};

const struct glyph_info g_W = {
    GLYPH_MONO(4, 5),
    {
        p(X, 0, 0, 0, X, 0, 0, 0),
        p(X, 0, 0, 0, X, 0, 0, 0),
        p(X, 0, X, 0, X, 0, 0, 0),
        p(0, X, 0, X, 0, 0, 0, 0),
    }
};

const struct glyph_info g_V = {
    GLYPH_MONO(3, 3),
    {
        p(X, 0, X, 0, 0, 0, 0, 0),
        p(X, 0, X, 0, 0, 0, 0, 0),
        p(0, X, 0, 0, 0, 0, 0, 0),
    }
};

const struct glyph_info g_O = {
    GLYPH_MONO(4, 5),
    {
        p(0, X, X, X, 0, 0, 0, 0),
        p(X, 0, 0, 0, X, 0, 0, 0),
        p(X, 0, 0, 0, X, 0, 0, 0),
        p(0, X, X, X, 0, 0, 0, 0),
    }
};

const struct glyph_info g_degF = {
    GLYPH_MONO(5, 6),
    {
        p(X, X, 0, X, X, X, 0, 0),
        p(X, X, 0, X, 0, 0, 0, 0),
        p(0, 0, 0, X, X, 0, 0, 0),
        p(0, 0, 0, X, 0, 0, 0, 0),
        p(0, 0, 0, X, 0, 0, 0, 0),
    }
};

const struct glyph_info g_E = {
    GLYPH_MONO(5, 3),
    {
        p(X, X, X, 0, 0, 0, 0, 0),
        p(X, 0, 0, 0, 0, 0, 0, 0),
        p(X, X, X, 0, 0, 0, 0, 0),
        p(X, 0, 0, 0, 0, 0, 0, 0),
        p(X, X, X, 0, 0, 0, 0, 0),
    }
};

const struct glyph_info g_psi = {
    GLYPH_MONO(4, 7),
    {
        p(X, X, X, X, X, 0, X, 0),
        p(X, 0, X, X, 0, 0, X, 0),
        p(X, X, 0, 0, X, 0, X, 0),
        p(X, 0, 0, X, X, 0, X, 0),
    }
};

const struct glyph_info g_lowbeam = {
    GLYPH_MONO(5, 6),
    {
        p(0, X, X, _, _, _, 0, 0),
        p(X, X, X, _, X, _, 0, 0),
        p(X, X, X, _, _, X, 0, 0),
        p(X, X, X, _, X, _, 0, 0),
        p(0, X, X, _, _, X, 0, 0),
    }
};

const struct glyph_info g_highbeam = {
    GLYPH_MONO(5, 6),
    {
        p(0, X, X, _, X, X, 0, 0),
        p(X, X, X, _, _, _, 0, 0),
        p(X, X, X, _, X, X, 0, 0),
        p(X, X, X, _, _, _, 0, 0),
        p(0, X, X, _, X, X, 0, 0),
    }
};

const struct glyph_info g_left_triangle = {
    GLYPH_MONO(7, 4),
    {
        p(0, 0, 0, X, 0, 0, 0, 0),
        p(0, 0, X, X, 0, 0, 0, 0),
        p(0, X, X, X, 0, 0, 0, 0),
        p(X, X, X, X, 0, 0, 0, 0),
        p(0, X, X, X, 0, 0, 0, 0),
        p(0, 0, X, X, 0, 0, 0, 0),
        p(0, 0, 0, X, 0, 0, 0, 0),
    }
};

const struct glyph_info g_right_triangle = {
    GLYPH_MONO(7, 4),
    {
        p(X, 0, 0, 0, 0, 0, 0, 0),
        p(X, X, 0, 0, 0, 0, 0, 0),
        p(X, X, X, 0, 0, 0, 0, 0),
        p(X, X, X, X, 0, 0, 0, 0),
        p(X, X, X, 0, 0, 0, 0, 0),
        p(X, X, 0, 0, 0, 0, 0, 0),
        p(X, 0, 0, 0, 0, 0, 0, 0),
    }
};

