/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */


#include "mc33972.h"
#include "board.h"

#include "switches.h"

#include "lin_defs.h"

using namespace Master;

namespace Switches
{

struct Debounce {
    uint8_t     count: 7;   //< counts down to 0
    uint8_t     state: 1;   //< current state of the switch
};

static const uint8_t    kDebounceCycles = 5; // XXX needs to be computed/tuned
static const uint8_t    kStateBytes = (input_assignment::kNumEncodings + 7) / 8;
static Debounce         _state[input_assignment::kNumEncodings];

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
changed_to_on(uint8_t id)
{
    return changed(id) && test(id);
}

bool
changed_to_off(uint8_t id)
{
    return changed(id) && !test(id);
}

bool
changed()
{
    for (uint8_t id = 0; id < input_assignment::kNumEncodings; id++) {
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

    // XXX should use a bitarray
#define SET(x)  do { if (x < input_assignment::kNumEncodings) rawstate[x / 8] |= (1 << (x & 0x7)); } while(0)
#define GET(x)  ((rawstate[x / 8] & (1 << (x & 0x7))) ? 1 : 0)

    // fetch the switch inputs
    MC33972::scan();

    // hardcoded ignition input on SP0
    if (MC33972::test(MC33972::kInputSP0)) {
        SET(input_assignment::kIgnition);
    }

    // SP1-SP7
    for (uint8_t sw = 0; sw <= 6; sw++) {
        if (MC33972::test(MC33972::kInputSP1 + sw)) {
            SET(Parameter(kParamSP1Assign + sw).get());
        }
    }

    // SG0-SG13
    for (uint8_t sw = 0; sw <= 13; sw++) {
        if (MC33972::test(MC33972::kInputSG0 + sw)) {
            SET(Parameter(kParamSG0Assign + sw).get());
        }
    }

    // debounce raw state
    for (uint8_t i = 0; i < input_assignment::kNumEncodings; i++) {

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
