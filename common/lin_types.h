
#pragma once
#include <stdint.h>

class Encoding
{
public:
    static bool                 invalid(uint8_t encoding, uint16_t value);
    static const PROGMEM char   *name(uint8_t encoding);
    static const PROGMEM char   *info(uint8_t encoding, uint16_t value);
};

class Response
{
public:
    Response() {}
    Response(uint8_t b0,
             uint8_t b1 = 0,
             uint8_t b2 = 0,
             uint8_t b3 = 0,
             uint8_t b4 = 0,
             uint8_t b5 = 0,
             uint8_t b6 = 0,
             uint8_t b7 = 0) :
        _bytes{b0, b1, b2, b3, b4, b5, b6, b7}
    {}
    Response(Response &r) :
        _bytes{r[0], r[1], r[2], r[3], r[4], r[5], r[6], r[7]}
    {}

    uint8_t &operator[](uint8_t index) { return _bytes[index]; }

protected:
    uint8_t         _bytes[8];
};

template<typename T, unsigned OFFSET, unsigned LENGTH, unsigned ENCODING>
class Signal
{
public:
    Signal(Response &r) : _r(r) {}

    operator T () const { return get(); }
    void operator = (T v) { set(v); }
    bool            test() const { return get() != 0; }

    void            set(T value) {
        if (LENGTH == 1) {
            // single bit set
            if (value) {
                _r[_byteoffset] |= (1 << _bitoffset);
            } else {
                _r[_byteoffset] &= ~(1 << _bitoffset);
            }
        } else if ((LENGTH == 8) && (_bitoffset == 0)) {
            // aligned byte set
            _r[_byteoffset] = value;
        } else if ((LENGTH == 16) && (_bitoffset == 0)) {
            // byte-aligned word set
            _r[_byteoffset] = value & 0xff;
            _r[_byteoffset + 1] = value >> 8;
        } else {
            // arbitrary bit range set
            uint8_t size = LENGTH;
            uint8_t offset = OFFSET;

            while (size) {
                if (value & 1) {
                    _r[offset / 8] |= (1 << (offset % 8));
                } else {
                    _r[offset / 8] &= ~(1 << (offset % 8));
                }
                size -= 1;
                offset += 1;
                value >>= 1;
            }
        }
    }

    T               get() const {
        if (LENGTH == 1) {
            // single bit get
            return _r[_byteoffset] & (1 << _bitoffset); 
        } else if ((LENGTH == 8) && (_bitoffset == 0)) {
            // aligned byte get
            return _r[_byteoffset];
        } else if ((LENGTH == 16) && (_bitoffset == 0)) {
            // byte-aligned word get
            return _r[_byteoffset] + (T)(_r[_byteoffset + 1]) << 8;
        } else {
            uint8_t size = LENGTH;
            uint8_t offset = OFFSET + LENGTH - 1;
            T value = 0;

            while (size) {
                value <<= 1;
                if (_r[offset / 8] & (1 << (offset %8))) {
                    value |= 1;
                }

                size -= 1;
                offset -= 1;
            }
            return value;
        }
    }

    uint8_t         encoding() const { return ENCODING; }

private:
    static const uint8_t    _byteoffset = OFFSET / 8;
    static const uint8_t    _bitoffset = OFFSET % 8;
    static const uint8_t    _mask = ((1 << LENGTH) - 1) << _bitoffset;
    Response                &_r;
};

class Parameter
{
public:
    typedef uint16_t Address;
    typedef void    (* Defaulter)(const Parameter &param);

    static const Address    noAddress = 0xffff;

    constexpr Parameter(Address address, uint8_t encoding, Defaulter defaulter) :
        _address(address),
        _encoding(encoding),
        _defaulter(defaulter)
    {
    }

    void            set(uint16_t value) const;  // must be implemented by the device
    uint16_t        get() const;                // must be implemented by the device

    uint16_t        address() const { return _address; }

    void            init() const 
    {
        if (is_invalid(get())) {
            _defaulter(*this);
        }
    }

    bool            exists() const { return (address() != noAddress); }

    bool            is_invalid(uint16_t value) const
    {
        return Encoding::invalid(_encoding, value);
    }

    bool            prev(uint16_t &value, uint16_t decrement = 1) const
    {
        uint16_t new_value = value;

        while (new_value >= decrement) {
            new_value -= decrement;
            if (!is_invalid(new_value)) {
                value = new_value;
                return true;
            }
        }
        return false;
    }

    bool            next(uint16_t &value, uint16_t increment = 1) const
    {
        uint16_t new_value = value;

        while ((new_value + increment) > new_value) {
            new_value += increment;
            if (!is_invalid(new_value)) {
                value = new_value;
                return true;
            }
        }
        return false;
    }

private:
    const Address   _address;
    const uint8_t   _encoding;
    const Defaulter _defaulter;
};
