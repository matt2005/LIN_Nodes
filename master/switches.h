///@file switches.h

#pragma once

#include "lin_protocol.h"
#include "mc33972.h"

class Switches
{
public:
    void            scan();

    bool operator[](LIN::SwitchID id) const 
    {
        return _cache[id / 8] & (1 << (id &0x7));
    }

private:
    static const uint8_t kCacheSlots = (LIN::kSWMax + 7) / 8;

    MC33972         _inputs;

    uint8_t         _cache[kCacheSlots];
};
