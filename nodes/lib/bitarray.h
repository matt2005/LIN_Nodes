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
    StaticBitarray() : Bitarray<N>(_buffer), _buffer{0} {}
    StaticBitarray(Bitarray<N> &orig) : Bitarray<N>(_buffer), _buffer(orig._buffer) {}

private:
    static const uint8_t    kSizeBytes = (N + 7) / 8;

    uint8_t                 _buffer[kSizeBytes];
};
