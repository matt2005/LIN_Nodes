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

    operator T & () { return _value; }
    T & operator++() { increment(); return _value; }

private:
    T               _value;
};

typedef Counter<uint8_t, 255>       Counter8;
typedef Counter<uint16_t, 65535>    Counter16;

} // namespace Util
