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
