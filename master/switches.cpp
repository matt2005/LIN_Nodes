#include "switches.h"

Switches::Switches()
{
    MC33972 inputs;

    inputs.init();    
}

void
Switches::scan()
{
    // clear raw state
    uint8_t rawstate[kStateBytes];
    for (uint8_t i = 0; i < kStateBytes; i++) {
        rawstate[i] = 0;
    }

    #define SET(x)  do { if (x < LIN::kSWMax) rawstate[x / 8] |= (1 << (x & 0x7)); } while(0)
    #define GET(x)  ((rawstate[x / 8] & (1 << (x & 0x7))) ? 1 : 0)

    // fetch the switch inputs
    {
        MC33972 inputs;

        inputs.scan();

        // hardcoded ignition input on SP0
        if (inputs[MC33972::kInputSP0]) {
            SET(LIN::kSWIgnition);
        }

        // SP1-SP7
        for (uint8_t sw = 1; sw <=7; sw++) {
            if (inputs[MC33972::kInputSP0 + sw]) {
                SET(paramSPAssign(sw).get());
            }
        }

        // SG0-SG13
        for (uint8_t sw = 0; sw <= 13; sw++) {
            if (inputs[MC33972::kInputSG0 + sw]) {
                SET(paramSGAssign(sw).get());
            }
        }
    }

    // debounce raw state
    for (uint8_t i = 0; i < LIN::kSWMax; i++) {

        // if no change, reset debounce timer (also clears
        // 'changed' state
        if (_state[i].state == GET(i)) {
            _state[i].count = 0;
            continue;
        }

        // switch does not match state; is timer running?
        if (_state[i].count == 0) {
            // no, start the debounce timer
            _state[i].count = 100;  // XXX needs to be computed/tuned
            continue;
        }

        // decrement timer
        _state[i].count--;

        // if this is the last count, change the 'current' state;
        // last count value will be cleared by reset path above
        if (_state[i].count == 1) {
            _state[i].state = GET(i);
        }
    }
}
