/// @file parameter.h

#pragma once

#include <avr/eeprom.h>

class Parameter
{
public:
    static const uint8_t    maxIndex = UINT8_MAX;

    constexpr Parameter(uint8_t index,
                        uint8_t defval = 0xff,
                        uint8_t max = 0xff) :
        _index(index),
        _defval(defval),
        _max(max)
    {
    }

    uint8_t     get() const
    {
        return eeprom_read_byte((uint8_t *)0 + _index);
    }
    void        set(uint8_t value) const
    {
        eeprom_update_byte((uint8_t *)0 + _index, (value <= _max) ? value : _max);
    }
    void        init() const 
    {
        set(get());
    }
    uint8_t     index() const
    {
        return _index;
    }

private:
    const uint8_t   _index;
    const uint8_t   _defval;
    const uint8_t   _max;
};
