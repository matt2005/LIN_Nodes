#include <avr/pgmspace.h>

#include "board.h"
#include "lin_protocol.h"
#include "mc33972.h"

#include "hd44780.h"
#include "menu.h"

#include "m_top.h"

namespace Menu
{

static Mode         *_mode;     ///< current mode

////////////////////////////////////////////////////////////////////////////////
// Toplevel menu wrapper
//
void
tick()
{
    Mode *newmode;

    // kick the mode state machine
    if (_mode != nullptr) {
        newmode = _mode->tick();
    } else {
        newmode = &modeTop;
    }

    // mode change?
    if (newmode != _mode) {
        newmode->action(Encoder::kEventActivate);
        _mode = newmode;
    }
}

Mode *
Mode::tick()
{
    return action(Encoder::event());
}

} // namespace Menu
