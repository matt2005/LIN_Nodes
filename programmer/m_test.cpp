
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
TestMode::action(Encoder::Event bp)
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
