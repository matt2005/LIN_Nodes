/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <avr/pgmspace.h>

#include "board.h"

#include "hd44780.h"
#include "m_top.h"
#include "slave.h"

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
        gDisplay.printf(PSTR("proto %3u"), Generic::parameter(Generic::kParamProtocolVersion));
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
