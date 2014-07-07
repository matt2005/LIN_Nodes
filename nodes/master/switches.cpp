
#include "mc33972.h"
#include "lin_protocol.h"
#include "board.h"

#include "switches.h"

#include "protocol.h"
#include "param_Master.h"

namespace Switches
{

struct Debounce {
    uint8_t     count: 7;   //< counts down to 0
    uint8_t     state: 1;   //< current state of the switch
};

static const uint8_t    kDebounceCycles = 5; // XXX needs to be computed/tuned
static const uint8_t    kStateBytes = (LIN::kSwitchIDMax + 7) / 8;
static Debounce         _state[LIN::kSwitchIDMax];

void
init()
{
    MC33972::configure();
//#ifdef DEBUG
//
//    for (uint8_t sw = 1; sw <= 7; sw++) {
//        debug("SP%2u: %2u", sw, paramSPAssign(sw).get());
//    }
//
//    for (uint8_t sw = 0; sw <= 13; sw++) {
//        debug("SG%2u: %2u", sw, paramSGAssign(sw).get());
//    }
//
//#endif
}

bool
test(uint8_t id)
{
    return _state[id].state;
}

bool
changed(uint8_t id)
{
    return _state[id].count == 1;
}

bool
changedToOn(uint8_t id)
{
    return changed(id) && test(id);
}

bool
changedToOff(uint8_t id)
{
    return changed(id) && !test(id);
}

bool
changed()
{
    for (uint8_t id = 0; id < LIN::kSwitchIDMax; id++) {
        if (changed(id)) {
            return true;
        }
    }

    return false;
}

void
scan()
{
    // clear raw state
    uint8_t rawstate[kStateBytes];

    for (uint8_t i = 0; i < kStateBytes; i++) {
        rawstate[i] = 0;
    }

#define SET(x)  do { if (x < LIN::kSwitchIDMax) rawstate[x / 8] |= (1 << (x & 0x7)); } while(0)
#define GET(x)  ((rawstate[x / 8] & (1 << (x & 0x7))) ? 1 : 0)

    // fetch the switch inputs
    MC33972::scan();

    // hardcoded ignition input on SP0
    if (MC33972::test(MC33972::kInputSP0)) {
        SET(LIN::kSwitchIDIgnition);
    }

    // SP1-SP7
    for (uint8_t sw = 0; sw <= 6; sw++) {
        if (MC33972::test(MC33972::kInputSP1 + sw)) {
            SET(Parameter(kParamSPAssign1 + sw));
        }
    }

    // SG0-SG13
    for (uint8_t sw = 0; sw <= 13; sw++) {
        if (MC33972::test(MC33972::kInputSG0 + sw)) {
            SET(Parameter(kParamSGAssign0 + sw));
        }
    }

    // debounce raw state
    for (uint8_t i = 0; i < LIN::kSwitchIDMax; i++) {

        // if no change, reset debounce timer (also clears
        // 'changed' state
        if (_state[i].state == GET(i)) {
            _state[i].count = 0;
            continue;
        }

        // switch does not match state; is timer running?
        if (_state[i].count == 0) {
            // no, start the debounce timer
            _state[i].count = kDebounceCycles;
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

} // namespace Switches
