
#include <avr/pgmspace.h>

#include "board.h"

#include "hd44780.h"
#include "m_test.h"
#include "m_top.h"
#include "slave.h"

namespace Menu
{

TestMode modeTest;

////////////////////////////////////////////////////////////////////////////////
// Test mode
//

void
TestMode::enter(Mode *from)
{
    gDisplay.clear();
    gDisplay.printf(PSTR(">back"));
    gSlave.setSuspend(true);
}

Mode *
TestMode::action(Encoder::Event bp)
{
    switch (bp) {

    case Encoder::kEventPress:
        gSlave.setSuspend(false);
        return &modeTop;

    default:
        break;
    }

    return this;
}

} // namespace Menu
