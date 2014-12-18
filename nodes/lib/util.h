/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
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
