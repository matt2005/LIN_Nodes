
#include <avr/pgmspace.h>

#include "board.h"

#include "hd44780.h"
#include "m_top.h"
#include "slave.h"

#include "protocol.h"

namespace Menu
{

////////////////////////////////////////////////////////////////////////////////
// Info mode
//
// +--------------------+
// |                    |
// |                    |
// |                    |
// |                    |
// +--------------------+
//
#define FREEZE_STR(s) #s
#define STRINGIFY(s) FREEZE_STR(s)
#define GIT_STR PSTR(STRINGIFY(GIT_VERSION))

Mode *
InfoMode::action(Encoder::Event bp)
{
    switch (bp) {

    case Encoder::kEventPress:
        gSlave.set_suspend(false);
        return &modeTop;

    case Encoder::kEventActivate:
        gDisplay.clear();
        gDisplay.printf(PSTR("build %s"), GIT_STR);
        gDisplay.move(0, 1);
        gDisplay.printf(PSTR("proto %3u"), protocolRevision);
        gDisplay.move(0, 2);
        gDisplay.printf(PSTR("free  %3u"), Board::freemem());
        gSlave.set_suspend(true);
        break;

    default:
        break;
    }

    return this;
}

} // namespace Menu
