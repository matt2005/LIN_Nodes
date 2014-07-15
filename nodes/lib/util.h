///@file util.h

#pragma once

#include <stdint.h>

namespace Util
{

extern const char *strtab(const char *table, uint8_t index);
extern uint8_t strtablen(const char *table);

class Counter8
{
public:
    void            reset() { _value = 0; }
    void            increment() { if (_value < 255) _value++; }

    operator uint8_t & ()           { return _value; }
    uint8_t & operator++()          { increment(); return _value; }

private:
    uint8_t         _value;
};

} // namespace Util
