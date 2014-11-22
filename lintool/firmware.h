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

#include "exceptions.h"

class Firmware
{
public:
    Firmware(const char *fromFile);
    ~Firmware();

    EXCEPTION(Exception, ExIOFailed);
    EXCEPTION(Exception, ExLookup);

    unsigned        max_address() const { return _bytes.rbegin()->first; }
    bool            get_bytes(unsigned base, unsigned count, uint8_t *buf) const;
    const char     *function_name() const { return _functionName; }

    static Firmware *for_function(unsigned function);
    static Firmware *implied_firmware();

private:
    static std::list<Firmware *>    _firmwares;

    const char                      *_functionName;
    std::map<unsigned, uint8_t>     _bytes;
};
