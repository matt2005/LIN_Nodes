/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

///@file encoder.h

#pragma once

namespace Encoder
{

enum Event : uint8_t {
    kEventNone,         // no event
    kEventDown,         // encoder 'down'
    kEventUp,           // encoder 'up'
    kEventPress,        // encoder button press

    kEventActivate      // meta-event for mode entry
};

void init();
Event event();
void discard();

} // namespace Encoder
