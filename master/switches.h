///@file switches.h

#pragma once

#include "lin_protocol.h"
#include "mc33972.h"

class Switches
{
public:
    Switches();
    
    void            scan();

    bool test(LIN::SwitchID id) const {
        return _state[id].state;
    }

    bool changed(LIN::SwitchID id) const {
        return _state[id].count == 1;
    }

private:
    static const uint8_t kStateBytes = (LIN::kSWMax + 7) / 8;

    struct Debounce
    {
        uint8_t     count:7;    //< counts down to 0
        uint8_t     state:1;    //< current state of the switch
    };

    Debounce        _state[LIN::kSWMax];
};
