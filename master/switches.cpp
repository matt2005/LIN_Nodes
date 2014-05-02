#include "switches.h"

Switches::Switches()
{
    MC33972 inputs;

    inputs.init();    
}

void
Switches::scan()
{
    MC33972 inputs;

    // fetch the switch inputs
    inputs.scan();

    for (uint8_t i = 0; i < kCacheSlots; i++) {
        _cache[i] = 0;
    }

    // XXX debounce

    // hardcoded ignition input on SP0
    if (inputs[MC33972::kInputSP0]) {
        set(LIN::kSWIgnition);
    }

    // SP1-SP7
    for (uint8_t sw = 1; sw <=7; sw++) {
        if (inputs[MC33972::kInputSP0 + sw]) {
            set(Parameter(sw).get());
        }
    }

    // SG0-SG13
    for (uint8_t sw = 0; sw <= 13; sw++) {
        if (inputs[MC33972::kInputSG0 + sw]) {
            set(Parameter(8 + sw).get());
        }
    }
}
