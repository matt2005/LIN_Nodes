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
    // lazily init to idle mode
    if (_mode == nullptr) {
        _mode = &modeTop;
        _mode->enter(nullptr);
    }

    // kick the mode state machine
    Mode *newmode = _mode->tick();

    // mode change?
    if (newmode != _mode) {
        newmode->enter(_mode);
        _mode = newmode;
    }
}

Mode *
Mode::tick()
{
    return action(Encoder::event());
}

} // namespace Menu
