
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
    switch (bp) {

    case Encoder::kEventPress:
        return &modeExploreTest;

    case Encoder::kEventActivate:
        gDisplay.clear();
        gDisplay.printf(PSTR(">back"));
        break;

    default:
        break;
    }

    return this;
}

} // namespace Menu
