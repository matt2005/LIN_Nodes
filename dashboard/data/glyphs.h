/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
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
