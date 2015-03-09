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

#include "panel.h"
#include "lpc111x.h"

static const uint16_t   _bR1 = GPIO_IO_P0;
static const uint16_t   _bG1 = GPIO_IO_P1;
static const uint16_t   _bB1 = GPIO_IO_P2;
static const uint16_t   _bR2 = GPIO_IO_P6;
static const uint16_t   _bG2 = GPIO_IO_P7;
static const uint16_t   _bB2 = GPIO_IO_P8;
static const uint16_t   _bA = GPIO_IO_P3;   // to be shared with R1
static const uint16_t   _bB = GPIO_IO_P9;   // to be shared with G1
static const uint16_t   _bC = GPIO_IO_P11;  // to be shared with B1
static const uint16_t   _bD = _bR2;         // shared with R2

static const uint16_t   _bCLK = GPIO_IO_P4;
static const uint16_t   _bLAT = GPIO_IO_P5;
static const uint16_t   _bOE = GPIO_IO_P8;

#define GPIO_GPIO0_BITS(_x)     (*((REG32 *) (GPIO_GPIO0_BASE + ((_x) << 2))))
#define GPIO_GPIO1_BITS(_x)     (*((REG32 *) (GPIO_GPIO1_BASE + ((_x) << 2))))

#define GPIO_RGB_BITS       GPIO_GPIO0_BITS(GPIO_IO_P0 | \
        GPIO_IO_P1 | \
        GPIO_IO_P2 | \
        GPIO_IO_P6 | \
        GPIO_IO_P7 | \
        GPIO_IO_P8)

#define GPIO_RGB1_BITS      GPIO_GPIO0_BITS(GPIO_IO_P0 | \
        GPIO_IO_P1 | \
        GPIO_IO_P2)

#define GPIO_RGB2_BITS      GPIO_GPIO0_BITS(GPIO_IO_P6 | \
        GPIO_IO_P7 | \
        GPIO_IO_P8)


#define GPIO_ADDR_BITS      GPIO_GPIO0_BITS(GPIO_IO_P3 | GPIO_IO_P9 | GPIO_IO_P11 | GPIO_IO_P6)

#define GPIO_CLK_BITS       GPIO_GPIO1_BITS(GPIO_IO_P4)
#define GPIO_LAT_BITS       GPIO_GPIO1_BITS(GPIO_IO_P5)
#define GPIO_OE_BITS        GPIO_GPIO1_BITS(GPIO_IO_P8)
#define GPIO_CTL_BITS       GPIO_GPIO1_BITS(GPIO_IO_P4 | GPIO_IO_P5 | GPIO_IO_P8)

void
Panel::line_init()
{
    SCB_SYSAHBCLKCTRL |= SCB_SYSAHBCLKCTRL_GPIO | SCB_SYSAHBCLKCTRL_IOCON;

    IOCON_nRESET_PIO0_0 = IOCON_nRESET_PIO0_0_FUNC_GPIO;            // R1
    IOCON_PIO0_1 = IOCON_PIO0_1_FUNC_GPIO;                          // G1
    IOCON_PIO0_2 = IOCON_PIO0_2_FUNC_GPIO;                          // B1
    IOCON_PIO0_6 = IOCON_PIO0_6_FUNC_GPIO;                          // R2 / D
    IOCON_PIO0_7 = IOCON_PIO0_7_FUNC_GPIO;                          // G2
    IOCON_PIO0_8 = IOCON_PIO0_8_FUNC_GPIO;                          // B2
    IOCON_PIO0_3 = IOCON_PIO0_3_FUNC_GPIO;                          // A
    IOCON_PIO0_9 = IOCON_PIO0_9_FUNC_GPIO;                          // B
    IOCON_JTAG_TDI_PIO0_11 = IOCON_JTAG_TDI_PIO0_11_FUNC_GPIO |     // C
                             IOCON_JTAG_TDI_PIO0_11_ADMODE_DIGITAL;

    unsigned p0bits = _bR1 |
                      _bG1 |
                      _bB1 |
                      _bR2 |
                      _bG2 |
                      _bB2 |
                      _bA  |
                      _bB  |
                      _bC;

    GPIO_GPIO0DATA &= ~p0bits;
    GPIO_GPIO0DIR |= p0bits;

    IOCON_PIO1_4 = IOCON_PIO1_4_FUNC_GPIO |                         // CLK
                   IOCON_PIO1_4_ADMODE_DIGITAL;
    IOCON_PIO1_5 = IOCON_PIO1_5_FUNC_GPIO;                          // LAT
    IOCON_PIO1_8 = IOCON_PIO1_8_FUNC_GPIO;                          // OE

    unsigned p1bits = _bCLK |
                      _bLAT |
                      _bOE;

    GPIO_GPIO1DATA &= ~p1bits;
    line_off();
    GPIO_GPIO1DIR |= p1bits;
}

void
Panel::line_off()
{
    GPIO_OE_BITS = _bOE;
}

void
Panel::line_update(unsigned row, unsigned slot)
{
    line_off();

    uint32_t *lcp = &select_buffer(_fb_active)->cell(row * FrameBuffer::columns()).raw();
    uint32_t *hcp = lcp + FrameBuffer::rows() / 2 * FrameBuffer::columns() / Cell::stride();

    // ~33us for 32 columns
    for (unsigned col = 0; col < FrameBuffer::columns(); col += Cell::stride()) {
        uint32_t low_cell = *lcp++;
        uint32_t high_cell = *hcp++;

        // ~42 cycles per iteration
        for (unsigned subcol = 0; subcol < 8; subcol++) {
            GPIO_CLK_BITS = 0;                              // CLK low

            GPIO_RGB1_BITS = palette[low_cell & 0xf].slice(slot);
            GPIO_RGB2_BITS = palette[high_cell & 0xf].slice(slot) << 6;
            //GPIO_RGB_BITS = palette[low_cell & 0xf].slice(slot) | (palette[high_cell & 0xf].slice(slot) << 6);
            low_cell >>= 4;
            high_cell >>= 4;

            GPIO_CLK_BITS = _bCLK;                          // latch on rising edge
        }
    }

    GPIO_ADDR_BITS = ((row & 1) ? _bA : 0) |                // set row address
                     ((row & 2) ? _bB : 0) |
                     ((row & 4) ? _bC : 0) |
                     ((row & 8) ? _bD : 0);

    GPIO_LAT_BITS = _bLAT;                                  // lat high to latch row
    GPIO_CTL_BITS = 0;                                      // oe, clk, lat low to enable display
}


