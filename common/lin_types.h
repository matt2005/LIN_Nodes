/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#pragma once
#include <stdint.h>

class Parameter
{
public:
    typedef uint16_t Address;

    static const Address    noAddress = 0xffff;

    constexpr Parameter(Address address = noAddress) :
        _address(address)
    {
    }

    Parameter &operator = (const Parameter &p) { _address = p.address(); return *this; }

    //operator uint16_t () const { return get(); }
    //const Parameter & operator = (uint16_t value) const { set(value); return *this; }

    void            set(uint16_t value) const;  // must be implemented by the device
    uint16_t        get() const;                // must be implemented by the device

    constexpr uint16_t address() const { return _address; }
    constexpr bool  exists() const { return _address != noAddress; }

private:
    Address         _address;
};
