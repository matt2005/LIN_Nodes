/// @file parameter.h

#pragma once

#include <avr/eeprom.h>

class Parameter
{
public:
    static const uint8_t    maxIndex = UINT8_MAX;

    constexpr Parameter(uint8_t index,
                        uint8_t min_val = 0,
                        uint8_t max_val = 0xff,
                        uint8_t def_val = 0xff) :
        _index(index),
        _min_val(min_val),
        _max_val(max_val),
        _def_val(def_val)
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
            set(_def_val);
        }
    }
    uint8_t     index() const
    {
        return _index;
    }
    bool        valid(uint8_t value) const
    {
        return ((value >= _min_val) && (value <= _max_val));
    }
private:
    const uint8_t   _index;
    const uint8_t   _min_val;
    const uint8_t   _max_val;
    const uint8_t   _def_val;

};
