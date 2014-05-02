///@file switches.h

#pragma once

#include "lin_protocol.h"
#include "mc33972.h"

class Switches
{
public:
    Switches();
    
    void            scan();

    bool operator[](LIN::SwitchID id) const {
        return (id < LIN::kSWMax) && (_cache[id / 8] & (1 << (id &0x7)));
    }

private:
    static const uint8_t kCacheSlots = (LIN::kSWMax + 7) / 8;

    uint8_t         _cache[kCacheSlots];

    void            set(uint8_t sid) {
        if (sid < LIN::kSWMax) {
            _cache[sid / 8] |= (1 << (sid & 0x7));
        }
    }
};
