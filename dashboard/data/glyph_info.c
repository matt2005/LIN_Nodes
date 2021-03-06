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

const struct glyph_info g_W = {
    GLYPH_MONO(4, 5),
    {
        p(X, 0, 0, 0, X, 0, 0, 0),
        p(X, 0, 0, 0, X, 0, 0, 0),
        p(X, 0, X, 0, X, 0, 0, 0),
        p(0, X, 0, X, 0, 0, 0, 0),
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

const struct glyph_info g_legend = {
    GLYPH_MONO(16, 7),
    {
        p(X, X, X, X, X, 0, X, 0),
        p(X, 0, X, X, 0, 0, X, 0),
        p(X, X, 0, 0, X, 0, X, 0),
        p(X, 0, 0, X, X, 0, X, 0),
        p(0, 0, 0, 0, 0, 0, 0, 0),
        p(0, 0, 0, 0, 0, 0, 0, 0),
        p(0, X, X, 0, X, X, X, 0),
        p(0, X, X, 0, X, 0, 0, 0),
        p(0, 0, 0, 0, X, X, 0, 0),
        p(0, 0, 0, 0, X, 0, 0, 0),
        p(0, 0, 0, 0, 0, 0, 0, 0),
        p(0, 0, 0, 0, 0, 0, 0, 0),
        p(0, 0, 0, X, 0, X, 0, 0),
        p(0, 0, 0, X, 0, X, 0, 0),
        p(0, 0, 0, X, 0, X, 0, 0),
        p(0, 0, 0, 0, X, 0, 0, 0),
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

