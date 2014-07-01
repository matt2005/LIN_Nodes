
#include <avr/pgmspace.h>

#include "board.h"
#include "print.h"

#include "m_edit.h"

namespace Menu
{

EditMode modeEdit;

////////////////////////////////////////////////////////////////////////////////
// Value edit mode
//

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
    gDisplay.move(_x, _y);

    if (_stringtab == nullptr) {
        gDisplay.printf(_fmt, (*_value));
    } else {
        gDisplay.printf(_fmt, Util::strtab(_stringtab, (*_value)));
    }
}

} // namespace Menu
