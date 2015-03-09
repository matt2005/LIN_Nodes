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
 * Universal glyph format.
 *
 * If the MSB is set in the info field, each element of data[] is one pixel in
 * 5551 RGBA format, R in the LSB.
 *
 * If the MSB is not set in the info field, each element of data[] is 8 pixels
 * on one row, CACACACACACACACA, with the LSB being the leftmost pixel.
 */
#pragma once

#include <stdint.h>

struct glyph_info {
    uint16_t info;
    uint16_t data[];
};

#define GLYPH_MONO(_rows, _columns) ((_rows << 8) | _columns)
#define GLYPH_RGB(_rows, _columns)  (0x8000 | (_rows << 8) | _columns)
#define GLYPH_WIDTH(_i)             (_i & 0xff)
#define GLYPH_HEIGHT(_i)            ((_i >> 8) & 0x7f)

#ifdef __cplusplus
extern "C" {
#endif
extern const struct glyph_info g_barseg;
extern const struct glyph_info g_denied;
extern const struct glyph_info g_x;
extern const struct glyph_info g_tick;
extern const struct glyph_info g_battery;
extern const struct glyph_info g_oildrop;
extern const struct glyph_info g_exclamation;
extern const struct glyph_info g_wrench;
extern const struct glyph_info g_barrel;
extern const struct glyph_info g_water;
extern const struct glyph_info g_lowbeam;
extern const struct glyph_info g_highbeam;
extern const struct glyph_info g_left_triangle;
extern const struct glyph_info g_right_triangle;

extern const struct glyph_info g_W;
extern const struct glyph_info g_O;
extern const struct glyph_info g_E;
extern const struct glyph_info g_legend;
#ifdef __cplusplus
}
#endif
