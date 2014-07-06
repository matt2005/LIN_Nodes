
#include <avr/pgmspace.h>

#include "board.h"
#include "util.h"

#include "hd44780.h"
#include "m_top.h"
#include "m_explore.h"
#include "m_test.h"

namespace Menu
{

TopMode modeTop;

struct topNode {
    const char *name;
    Mode *const mode;
};

static PROGMEM Mode *const nodes[] = {
    &modeExplore,
    &modeTest,
    &modeInfo
};

////////////////////////////////////////////////////////////////////////////////
// Top menu mode
//
// +--------------------+
// +LIN Setup Tool      |
// +   Setup            |
// + > Diagnostics <    |
// +   Info             |
// +--------------------+
//

Mode *
TopMode::action(Encoder::Event bp)
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
        if (_index < 2) {
            _index++;
            wantDraw = true;
        }

        break;

    case Encoder::kEventPress:
        return reinterpret_cast<Mode *>(pgm_read_ptr(&nodes[_index]));

    case Encoder::kEventActivate:
        _index = 0;
        wantDraw = true;
        break;

    default:
        break;
    }

    if (wantDraw) {
        draw();
    }

    return this;
}

void
TopMode::draw()
{
    gDisplay.clear();
    gDisplay.printf(PSTR("LIN Setup Tool"));
    gDisplay.move(3, 1);
    gDisplay.printf(PSTR("Setup"));
    gDisplay.move(3, 2);
    gDisplay.printf(PSTR("Diagnostics"));
    gDisplay.move(3, 3);
    gDisplay.printf(PSTR("Info"));
    
    gDisplay.move(1, _index + 1);
    gDisplay.printf(PSTR(">>"));
}

} // namespace Menu
