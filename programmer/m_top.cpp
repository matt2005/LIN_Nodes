
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

struct topNode
{
    const char *name;
    Mode *const mode;
};

static PROGMEM const char nametab[] = 
    "Configure\0"
    "Test\0"
    "\0";

static PROGMEM Mode * const nodes[] = 
{
    &modeExplore,
    &modeTest
};

static const uint8_t kMaxNode = sizeof(nodes) / sizeof(nodes[0]) - 1;

////////////////////////////////////////////////////////////////////////////////
// Top menu mode
//

void
TopMode::enter(Mode *from)
{
    _index = 0;
    draw();    
}

Mode *
TopMode::action(Encoder::Event bp)
{
    bool wantDraw = false;
    switch (bp) {

    case Encoder::kEventDown:
        if (_index > 0) {
            _index--;
            wantDraw = true;
        }
        break;

    case Encoder::kEventUp:
        if (_index < kMaxNode) {
            _index++;
            wantDraw = true;
        }
        break;

    case Encoder::kEventPress:
        return reinterpret_cast<Mode *>(pgm_read_ptr(&nodes[_index]));

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
    gDisplay.printf(Util::strtab(nametab, _index));

#ifdef DEBUG
    gDisplay.move(8, 1);
    gDisplay.printf(PSTR(" free %3u"), Board::freemem());
#endif
}

} // namespace Menu
