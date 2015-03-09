/*
 * Copyright (c) 2012-2015, Mike Smith, <msmith@purgatory.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * o Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include "mc33972.h"
#include "board.h"

#include "switches.h"

#include <lin_defs.h>
#include <bitarray.h>

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
    StaticBitarray<input_assignment::kNumEncodings> rawstate;
    rawstate.reset();

    // fetch the switch inputs
    MC33972::scan();

    // hardcoded ignition input on SP0
    if (MC33972::test(MC33972::kInputSP0)) {
        rawstate.set(input_assignment::kIgnition);
    }

    // SP1-SP7
    for (uint8_t sw = 0; sw <= 6; sw++) {
        if (MC33972::test(MC33972::kInputSP1 + sw)) {
            const uint8_t stride = kParamSP2Assign - kParamSP1Assign;
            rawstate.set(Parameter(kParamSP1Assign + (sw * stride)).get());
        }
    }

    // SG0-SG13
    for (uint8_t sw = 0; sw <= 13; sw++) {
        if (MC33972::test(MC33972::kInputSG0 + sw)) {
            const uint8_t stride = kParamSG1Assign - kParamSG0Assign;
            rawstate.set(Parameter(kParamSG0Assign + (sw * stride)).get());
        }
    }

    // debounce raw state - ignition is always assumed to be lowest input encoding
    for (uint8_t i = input_assignment::kIgnition; i < input_assignment::kNumEncodings; i++) {

        // if no change, reset debounce timer (also clears
        // 'changed' state
        if (_state[i].state == rawstate.test(i)) {
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
            _state[i].state = rawstate.test(i);
        }
    }
}

} // namespace Switches
