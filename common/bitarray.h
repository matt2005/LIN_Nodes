/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#pragma once

class Bitarray
{
public:
    Bitarray(uint8_t *bytes) : _bytes(bytes) {}

    void            set(uint8_t n) volatile     { _bytes[index(n)] |= (bit(n)); }
    void            clear(uint8_t n) volatile   { _bytes[index(n)] &= ~(bit(n)); }
    bool            test(uint8_t n) const       { return _bytes[index(n)] & (bit(n)); }

    // XXX optimise for specific offset/size combinations later

    template<typename T>
    void            insert(T n, uint8_t offset, uint8_t size = sizeof(T) * 8)
    {
        while (size) {
            if (n & 1) {
                set(offset);

            } else {
                clear(offset);
            }

            size -= 1;
            offset += 1;
            n >>= 1;
        }
    }

    template<typename T>
    T               extract(uint8_t offset, uint8_t size = sizeof(T) * 8)
    {
        offset += size - 1;
        T n = 0;

        while (size) {

            n <<= 1;

            if (test(offset)) {
                n |= 1;
            }

            size -= 1;
            offset -= 1;
        }

        return n;
    }

    uint8_t &operator[](uint8_t n)          { return _bytes[n]; }
    uint8_t &operator[](uint8_t n) volatile { return _bytes[n]; }
    uint8_t operator[](uint8_t n) const     { return _bytes[n]; }

protected:
    uint8_t                  *const _bytes;

    static uint8_t  index(uint8_t n) { return n / 8; }
    static uint8_t  bit(uint8_t n)   { return (1 << (n & 0x7)); }
};

template<unsigned N>
class StaticBitarray : public Bitarray
{
public:
    StaticBitarray() : Bitarray(_buffer) {}
    StaticBitarray(StaticBitarray<N> &orig) : Bitarray(_buffer) 
    {
        for (auto i = 0; i < kNumBytes; i++) {
            _buffer[i] = orig._buffer[i];
        }
    }

    void            reset() { for (uint8_t i = 0; i < kNumBytes; i++) _buffer[i] = 0; }

    bool operator == (const StaticBitarray<N> &other) const
    {
        for (auto i = 0; i < kNumBytes; i++) {
            if (_buffer[i] != other._buffer[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator != (const StaticBitarray<N> &other) const
    {
        return !(*this == other);
    }

private:
    static const uint8_t    kNumBytes = (N + 7) / 8;
    uint8_t                 _buffer[kNumBytes];
};
