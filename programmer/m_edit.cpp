
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

void
EditMode::enter(Mode *from)
{
    _from = from;
    if (*_value < _min) {
        *_value = _min;
    } else if (*_value > _max) {
        *_value = _max;
    }
    draw();
}

Mode *
EditMode::action(Encoder::Event bp)
{
    bool wantDraw = false;

    switch (bp) {

    case Encoder::kEventDown:
        if (*_value > _min) {
            (*_value)--;
            wantDraw = true;
        } else if (*_value == _min) {
            *_value = _max;
            wantDraw = true;
        }
        break;

    case Encoder::kEventUp:
        if (*_value < _max) {
            (*_value)++;
            wantDraw = true;
        } else if (*_value == _max) {
            *_value = _min;
            wantDraw = true;
        }
        break;

    case Encoder::kEventPress:
        return _from;

    default:
        break;
    }

    if (wantDraw) {
        draw();
    }

    return this;
}

void
EditMode::draw()
{
    gDisplay.move(_x, _y);
    if (_strings == nullptr) {
        gDisplay.printf(_fmt, *_value);
    } else {
        gDisplay.printf(_fmt, _strings + *_value);
    }
}

} // namespace Menu
