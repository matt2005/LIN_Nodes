
#include <avr/pgmspace.h>

#include "board.h"

#include "hd44780.h"
#include "m_test.h"
#include "m_top.h"

namespace Menu
{

TestMode modeTest;

////////////////////////////////////////////////////////////////////////////////
// Test menu mode
//

void
TestMode::enter(Mode *from)
{
}

Mode *
TestMode::action(Button bp)
{
    switch (bp) {

    case kButtonDown:
        break;

    case kButtonUp:
        break;

    case kButtonSelect:
        return &modeTop;

    default:
        break;
    }

    return this;
}

} // namespace Menu