
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
uint8_t         *EditMode::_value;
Display::Region EditMode::_region;
uint8_t         EditMode::_min;
uint8_t         EditMode::_max;
const char      *EditMode::_fmt;
const char      *EditMode::_stringtab;

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
    if (_stringtab == nullptr) {
        if ((*_value) < _max) {
            (*_value)++;

        } else if ((*_value) == _max) {
            (*_value) = _min;
        }

    } else {
        if ((*_value) < (Util::strtablen(_stringtab) - 1)) {
            (*_value)++;

        } else {
            (*_value) = 0;
        }
    }
}

void
EditMode::decrement()
{
    if ((*_value) > 0) {
        (*_value)--;

    } else {
        if (_stringtab == nullptr) {
            (*_value) = _max;

        } else {
            (*_value) = Util::strtablen(_stringtab) - 1;
        }
    }
}

void
EditMode::draw()
{
    gDisplay.clear(_region);
    gDisplay.move(_region.p);

    if (_stringtab == nullptr) {
        gDisplay.printf(_fmt, (*_value));

    } else {
        gDisplay.printf(Util::strtab(_stringtab, (*_value)));
    }
}

} // namespace Menu
