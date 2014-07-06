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

void
Mode::_msg(PGM_P hdr, PGM_P message)
{
    gDisplay.clear();
    gDisplay.printf(hdr);
    gDisplay.move(0, 1);

    uint8_t row = 1;
    char c;
    while ((c = pgm_read_byte(message++)) != 0) {
        if (c == '\n') {
            row++;
            gDisplay.move(0, row);
        } else {
            gDisplay.putc(c);
        }
    }
}

} // namespace Menu
