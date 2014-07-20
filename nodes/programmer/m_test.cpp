
#include <avr/pgmspace.h>

#include "board.h"

#include "hd44780.h"

#include "m_top.h"
#include "slave.h"

#include "protocol.h"

namespace Menu
{

Mode *
ExploreTestMode::select()
{
    switch (_node) {

    case LIN::kNodeAddressMaster:
        // XXX test mode for master?
        break;

    case LIN::kNodeAddressPowerBase ...(LIN::kNodeAddressPowerBase + 15):
        modeTest.init(_node);
        return &modeTest;

    default:
        break;
    }

    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////
// Test mode
//
// +--------------------+
// |                    |
// |                    |
// |                    |
// |                    |
// +--------------------+
//
Mode *
TestMode::action(Encoder::Event bp)
{
    bool wantDraw = false;

    switch (bp) {

    case Encoder::kEventUp:
        if (_index > 0) {
            _index--;
            wantDraw = true;
        }
        break;

    case Encoder::kEventDown:
        if ((_index + 4) < Util::strtablen(namesForLINError)) {
            _index++;
            wantDraw = true;
        }
        break;

    case Encoder::kEventPress:
        return &modeExploreTest;

    case Encoder::kEventActivate:
        _index = 0;
        wantDraw = true;
        break;

    default:
        break;
    }

    // refresh the display every second
    if (_refreshed.is_older_than(1000)) {
        wantDraw = true;
        _refreshed.update();
    }

    if (wantDraw) {
        draw();
    }
    return this;
}

void
TestMode::draw()
{
    gDisplay.clear();

    for (uint8_t row = 0; row < 4; row++) {
        if ((_index + row) >= Util::strtablen(namesForLINError)) {
            break;
        }

        uint16_t count = 55555U;
        gSlave.get_error_count(_nad, _index + row, count);
        gDisplay.move(0, row);
        gDisplay.printf(PSTR("%u %s"), count, Util::strtab(namesForLINError, _index + row));
    }
}

} // namespace Menu
