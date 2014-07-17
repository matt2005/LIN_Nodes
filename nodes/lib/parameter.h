/// @file parameter.h

#pragma once

#include <avr/pgmspace.h>
#include <avr/eeprom.h>

class Parameter
{
public:
    static const uint8_t    maxIndex = UINT8_MAX;

    constexpr Parameter(uint8_t index, const uint8_t *info) :
        _index(index),
        _info(info)
    {
    }

    operator uint8_t () const { return get(); }

    uint8_t     get() const
    {
        return eeprom_read_byte((uint8_t *)0 + _index);
    }
    void        set(uint8_t value) const
    {
        if (valid(value)) {
            eeprom_update_byte((uint8_t *)0 + _index, value);
        }
    }
    void        init() const 
    {
        if (!valid(get())) {
            set(def_value());
        }
    }
    uint8_t     index() const
    {
        return _index;
    }
    bool        valid(uint8_t value) const
    {
        return ((value >= min_value()) && (value <= max_value()));
    }
    uint8_t     min_value() const { return pgm_read_byte(_info + 0); }
    uint8_t     max_value() const { return pgm_read_byte(_info + 1); }
    uint8_t     def_value() const { return pgm_read_byte(_info + 2); }
private:
    const uint8_t   _index;
    const uint8_t   *_info;
};
