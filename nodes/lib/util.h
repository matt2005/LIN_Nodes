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

///@file util.h

#pragma once

#include <stdint.h>

namespace Util
{

extern const char *strtab(const char *table, uint8_t index);
extern uint8_t strtablen(const char *table);

template<typename T, unsigned MAX>
class Counter
{
public:
    void            reset() { _value = 0; }
    void            increment() { if (_value < MAX) _value++; }

    operator T &() { return _value; }
    T &operator++() { increment(); return _value; }

private:
    T               _value = 0;
};

typedef Counter<uint8_t, 255>       Counter8;
typedef Counter<uint16_t, 65535>    Counter16;

template<typename T>
T
intsafe_copy(const volatile T *unsafe)
{
    static_assert(sizeof(T) == 2, "only works for 2-byte types");

    auto bytes = reinterpret_cast<const volatile uint8_t *>(unsafe);
    uint8_t high, low;

    do {
        high = bytes[1];
        low = bytes[0];
    } while (high != bytes[1]);

    return (static_cast<T>(high) << 8) | low;
}

} // namespace Util
