
#include <avr/pgmspace.h>

#include "board.h"

#include "hd44780.h"
#include "m_top.h"
#include "m_explore.h"
#include "m_setup.h"
#include "m_test.h"

namespace Menu
{

TopMode modeTop;

struct topNode
{
    const char *name;
    Mode *const mode;
};

static PROGMEM const char sSetup[] = "Setup";
static PROGMEM const char sExplore[] = "Explore";
static PROGMEM const char sTest[] = "Test";

static PROGMEM const topNode nodes[] = 
{
    {sSetup,    &modeSetup},
    {sExplore,  &modeExplore},
    {sTest,     &modeTest}
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
TopMode::action(Button bp)
{
    bool wantDraw = false;
    switch (bp) {

    case kButtonDown:
        if (_index < kMaxNode) {
            _index++;
            wantDraw = true;
        }
        break;

    case kButtonUp:
        if (_index > 0) {
            _index--;
            wantDraw = true;
        }
        break;

    case kButtonSelect:
        return reinterpret_cast<Mode *>(pgm_read_ptr(&nodes[_index].mode));

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
    gDisplay.printf(reinterpret_cast<const char *>(pgm_read_ptr(&nodes[_index].name)));

#ifdef DEBUG
    gDisplay.move(8, 1);
    gDisplay.printf(PSTR("free %3u"), Board::freemem());
#endif
}

} // namespace Menu