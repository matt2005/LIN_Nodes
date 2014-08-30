/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */
#pragma once

#include <map>
#include <list>

#include <stdint.h>

class Firmware
{
public:
    Firmware(const char *fromFile);
    ~Firmware();

    unsigned        max_address() const { return _bytes.rbegin()->first; }
    bool            get_bytes(unsigned base, unsigned count, uint8_t *buf) const;

    static Firmware *for_function(unsigned function);

private:
    static std::list<Firmware *>    _firmwares;

    const char                      *_functionName;
    std::map<unsigned, uint8_t>     _bytes;
};
