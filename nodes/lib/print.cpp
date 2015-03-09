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

#include <stdarg.h>

#include <avr/pgmspace.h>

#include "print.h"
#include "util.h"

void
Print::printf(PGM_P fmt, ...)
{
    va_list ap;
    uint8_t c;

    va_start(ap, fmt);

    while ((c = pgm_read_byte(fmt++)) != 0) {

        // nl -> cr,nl
        if (c == '\n') {
            write('\r');
        }

        // non-format characters
        if (c != '%') {
            write(c);
            continue;
        }

        // initalise formatter
        uint8_t w = 0;

nextfmt:
        c = pgm_read_byte(fmt++);

        switch (c) {
        case '\0':                  // sanity
            return;

        case '1'...'9':
            w = (10 * w) + (c - '0');
            goto nextfmt;

        case 's':
            write(va_arg(ap, const char *), w);
            break;

        case 't': {
            const char *t = va_arg(ap, const char *);
            unsigned i = va_arg(ap, unsigned);
            write(Util::strtab(t, i), w);
        }
        break;

        case 'p':
            write('0');
            write('x');

        /* FALLTHROUGH */
        case 'x':
            writex(va_arg(ap, unsigned), w ? w : 4);
            break;

        case 'u':
            write(va_arg(ap, unsigned), w ? w : 5);
            break;

        default:
            break;
        }
    }
}

void
Print::write(uint16_t n, uint8_t width)
{
    char buf[width];
    uint8_t pos = width - 1;
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
        putc(buf[pos++]);
    }
}

void
Print::writex(uint16_t n, uint8_t width)
{
    uint8_t shift = 4 * (width - 1);

    for (;;) {
        uint8_t d = (n >> shift) & 0xf;

        if (d <= 9) {
            putc('0' + d);

        } else {
            putc('a' + d - 10);
        }

        if (shift == 0) {
            break;
        }

        shift -= 4;;
    }
}

void
Print::write(const char *s, uint8_t width)
{
    if (s == nullptr) {
        s = PSTR("null");
    }

    char c;

    while (width > strlen_PF((uint_farptr_t)s)) {
        putc(' ');
        width--;
    }

    while ((c = pgm_read_byte((uint_farptr_t)s++)) != '\0') {
        putc(c);
    }
}
