/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#pragma once

template <uint8_t N>
class Bitarray
{
public:
    Bitarray(uint8_t *bytes) : _bytes(bytes) {}

    void            set(uint8_t n) volatile     { if (n < kSizeBits)        _bytes[index(n)] |= (bit(n)); }
    void            clear(uint8_t n) volatile   { if (n < kSizeBits)        _bytes[index(n)] &= ~(bit(n)); }
    bool            test(uint8_t n) const       { return (n < kSizeBits) ? (_bytes[index(n)] & (bit(n))) : 0; }
    void            reset() { for (uint8_t i = 0; i < kSizeBytes; i++) _bytes[i] = 0; }

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

    bool operator == (const Bitarray<N> &other) const
    {
        for (auto i = 0; i < kSizeBytes; i++) {
            if (_bytes[i] != other._bytes[i]) {
                return false;
            }
        }

        return true;
    }

    bool operator != (const Bitarray<N> &other) const
    {
        return !(*this == other);
    }

    uint8_t operator[](uint8_t i) const { return (i < kSizeBytes) ? _bytes[i] : 0; }

protected:
    uint8_t                  *const _bytes;

    static uint8_t  index(uint8_t n) { return n / 8; }
    static uint8_t  bit(uint8_t n)   { return (1 << (n & 0x7)); }

private:
    static const uint8_t    kSizeBytes = (N + 7) / 8;
    static const uint8_t    kSizeBits = N;

};

template<uint8_t N>
class StaticBitarray : public Bitarray<N>
{
public:
    StaticBitarray() : Bitarray<N>(_buffer) {}
    StaticBitarray(Bitarray<N> &orig) : Bitarray<N>(_buffer)
    {
        for (auto i = 0; i < kSizeBytes; i++) {
            _buffer[i] = orig[i];
        }
    }

private:
    static const uint8_t    kSizeBytes = (N + 7) / 8;

    uint8_t                 _buffer[kSizeBytes];
};
