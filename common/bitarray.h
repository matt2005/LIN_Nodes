/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#pragma once

template<unsigned N>
class Bitarray
{
public:
    void            reset() { for (uint8_t i = 0; i < kNumBytes; i++) _bytes[i] = 0; }

    void            set(uint8_t n) volatile     { _bytes[index(n)] |=  (bit(n)); }
    void            clear(uint8_t n) volatile   { _bytes[index(n)] &= ~(bit(n)); }
    bool            test(uint8_t n) const       { return _bytes[index(n)] &   (bit(n)); }

    uint8_t &operator[](uint8_t n)          { return _bytes[n]; }
    uint8_t &operator[](uint8_t n) volatile { return _bytes[n]; }
    uint8_t operator[](uint8_t n) const     { return _bytes[n]; }

private:
    static const uint8_t    kNumBytes = (N + 7) / 8;
    uint8_t                 _bytes[kNumBytes];

    static uint8_t          index(uint8_t n) { return n / 8; }
    static uint8_t          bit(uint8_t n)   { return (1 << (n & 0x7)); }
};
