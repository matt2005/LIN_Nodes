///@file util.h

#pragma once

#include <stdint.h>

namespace Util
{

extern const char *strtab(const char *table, uint8_t index);
extern uint8_t strtablen(const char *table);

template<unsigned N>
class Bitarray
{
public:
    void            reset() { for (uint8_t i = 0; i < _nBytes; i++) _bytes[i] = 0; }

    void            set(uint8_t n) volatile      { _bytes[index(n)] |=  (bit(n)); }
    void            clear(uint8_t n) volatile    { _bytes[index(n)] &= ~(bit(n)); }
    bool            test(uint8_t n) const { return _bytes[index(n)] &   (bit(n)); }

    uint8_t &operator[](uint8_t n) volatile { return _bytes[n]; }
    uint8_t &operator[](uint8_t n)          { return _bytes[n]; }

private:
    static const uint8_t    _nBytes = (N + 7) / 8;
    uint8_t                 _bytes[_nBytes];

    static uint8_t          index(uint8_t n) { return n / 8; }
    static uint8_t          bit(uint8_t n)   { return (1 << (n & 0x7)); }
};

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
