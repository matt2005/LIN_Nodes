#include "switches.h"

void
Switches::scan()
{
    _inputs.scan();

    for (uint8_t i = 0; i < kCacheSlots; i++) {
        _cache[i] = 0;
    }

    for (uint8_t sw = 0; sw < _inputs.kInputMax; sw++) {
        if (_inputs[(MC33972::Input)sw]) {
            auto sid = (LIN::SwitchID)Parameter(sw).get();

            _cache[sid / 8] |= (1 << (sid & 0x7));
        }
    }
}
