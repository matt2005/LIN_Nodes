
#include <avr/pgmspace.h>

#include "board.h"

#include "hd44780.h"
#include "m_setup.h"
#include "m_top.h"

namespace Menu
{

SetupMode modeSetup;

////////////////////////////////////////////////////////////////////////////////
// Setup menu mode
//

void
SetupMode::enter(Mode *from)
{
}

Mode *
SetupMode::action(Encoder::Event bp)
{
    switch (bp) {

    case Encoder::kEventDown:
        break;

    case Encoder::kEventUp:
        break;

    case Encoder::kEventPress:
        return &modeTop;

    default:
        break;
    }

    return this;
}

} // namespace Menu
