
#include <avr/pgmspace.h>

#include "board.h"

#include "hd44780.h"
#include "m_test.h"
#include "m_top.h"
#include "slave.h"

namespace Menu
{

TestMode modeTest;
InfoMode modeInfo;

////////////////////////////////////////////////////////////////////////////////
// Test mode
//

Mode *
TestMode::action(Encoder::Event bp)
{
    switch (bp) {

    case Encoder::kEventPress:
        gSlave.setSuspend(false);
        return &modeTop;

    case Encoder::kEventActivate:
        gDisplay.clear();
        gDisplay.printf(PSTR(">back"));
        gSlave.setSuspend(true);
        break;

    default:
        break;
    }

    return this;
}

////////////////////////////////////////////////////////////////////////////////
// Info mode
//

#define FREEZE_STR(s) #s
#define STRINGIFY(s) FREEZE_STR(s)
#define GIT_STR PSTR(STRINGIFY(GIT_VERSION))

Mode *
InfoMode::action(Encoder::Event bp)
{
    switch (bp) {

    case Encoder::kEventPress:
        gSlave.setSuspend(false);
        return &modeTop;

    case Encoder::kEventActivate:
        gDisplay.clear();
        gDisplay.printf(PSTR("build %s"), GIT_STR);
        gDisplay.move(0, 1);
        gDisplay.printf(PSTR("free  %3u"), Board::freemem());
        gSlave.setSuspend(true);
        break;

    default:
        break;
    }

    return this;
}

} // namespace Menu
