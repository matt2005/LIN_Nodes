///@file encoder.h

#pragma once

namespace Encoder
{

enum Event : uint8_t {
    kEventNone,
    kEventDown,
    kEventUp,
    kEventPress
};

void init();
Event event();
void discard();

} // namespace Encoder
