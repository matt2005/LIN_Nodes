
#pragma once
#include <stdint.h>

namespace Encoding {
typedef bool    (* Validator)(uint16_t value);
} // namespace Encoding

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

template<typename T, unsigned OFFSET, unsigned LENGTH>
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
            uint8_t size = SIZE;
            uint8_t offset = OFFSET;

            while (size) {
                if (value & 1) {
                    _r[offset / 8] |= (1 << (offset % 8));
                } else {
                    _r[offset / 8] &= ~(1 << (offset % 8));
                }
                size -= 1;
                offset += 1;
                n >>= 1;
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
            uint8_t size = SIZE;
            uint8_t offset = OFFSET + SIZE - 1;
            T value = 0;

            while (size) {
                value <<= 1;
                if (_[offset / 8] & (1 << (offset %8))) {
                    value |= 1;
                }

                size -= 1;
                offset -= 1;
            }
            return value;
        }
    }

private:
    static const uint8_t    _byteoffset = OFFSET / 8;
    static const uint8_t    _bitoffset = OFFSET % 8;
    static const uint8_t    _mask = ((1 << LENGTH) - 1) << _bitoffset;
    Response                &_r;
};

class Parameter
{
public:
    struct ParamInfo {
        uint16_t            default_value;
    };

    constexpr Parameter(uint8_t page, uint8_t index, ParamInfo *info) :
        _page(page),
        _index(index),
        _info(info)
    {
    }

    void            set(uint16_t value) const;  // must be implemented by the device
    uint16_t        get() const;                // must be implemented by the device

    void            init() const 
    {
        if (!is_valid(get()) {
            set(default_value());
        }
    }

    bool            is_valid(uint16_t value) const
    {
        if (_info == nullptr) {
            return false;
        }
        Encoding::Validator validator = (Encoding::Validator)pgm_read_ptr(&((info + index)->is_valid));
        return validator(value);
    }

    bool            prev(uint16_t &value, uint16_t decrement = 1) const
    {
        uint16_t new_value = value;

        while (new_value >= decrement) {
            new_value -= decrement;
            if (valid(new_value)) {
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
            if (valid(new_value)) {
                value = new_value;
                return true;
            }
        }
        return false;
    }

private:
    uint8_t         _page;
    uint8_t         _index;
    ParamInfo       *_info;

    ParamInfo       *info() const
    {
        return _info + _index;
    }

    Encoding::Validator *validator)() const
    {
        return pgm_read_ptr(&(info()->validator));
    }

    uint16_t        default_value() const
    {
        return pgm_read_word(&(info()->default_value));
    }
};
