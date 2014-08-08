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
#include "print.h"

#include "m_top.h"

namespace Menu
{

////////////////////////////////////////////////////////////////////////////////
// Value edit mode
//

Mode            *EditMode::_from;
Display::Region EditMode::_region;
Parameter       *EditMode::_param;
uint16_t        *EditMode::_value;

Mode *
EditMode::action(Encoder::Event bp)
{
    switch (bp) {
    case Encoder::kEventDown:
        decrement();
        draw();
        break;

    case Encoder::kEventUp:
        increment();
        draw();
        break;

    case Encoder::kEventPress:
        return _from;

    case Encoder::kEventNone:
        break;

    default:
        draw();
    }

    return this;
}

void
EditMode::increment()
{
    _param->next(*_value, 1);
}

void
EditMode::decrement()
{
    _param->next(*_value, -1);
}

void
EditMode::draw()
{
    gDisplay.clear(_region);
    gDisplay.move(_region.p);

    const char *info = _param->info(*_value);
    if (info != nullptr) {
        gDisplay.printf(PSTR("%s"), info);
    } else {
        gDisplay.printf(PSTR("%u"), (*_value));
    }
}

} // namespace Menu
