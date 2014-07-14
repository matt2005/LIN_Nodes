
#include "util.h"
#include "board.h"

#include "lin_protocol.h"
#include "protocol.h"

#include "m_setup.h"
#include "m_explore.h"
#include "m_edit.h"
#include "slave.h"

namespace Menu
{

////////////////////////////////////////////////////////////////
// Node parameter editor
//
// +--------------------+
// |Title:              |
// |>>ParameterName     |
// |  ValueUnits        |
// |                    |
// +--------------------+
//
// +--------------------+
// |Title:              |
// |>>Back to node list |
// |                    |
// |                    |
// +--------------------+
//

Mode *
SetupMode::action(Encoder::Event bp)
{
    bool indexChanged = false;

    switch (bp) {
    case Encoder::kEventDown:
        if (_param > 0) {
            _param--;
        }

        indexChanged = true;
        break;

    case Encoder::kEventUp:
        if (_param < _max_param) {
            _param++;
        }

        indexChanged = true;
        break;

    case Encoder::kEventPress:
        switch (_param) {
        case 0:
            return &modeExplore;

        default:
            _editing = true;
            gDisplay.move(0, 1);
            gDisplay.printf(PSTR("  "));
            gDisplay.move(0, 2);
            gDisplay.printf(PSTR(">>"));
            // edit was already setup up at draw time
            return &modeEdit;
        }

        break;

    case Encoder::kEventActivate:
        if (_editing) {
            _editing = false;

            if (!gSlave.setParameter(_nad, _param, _value)) {
                gDisplay.clear();
                gDisplay.printf(PSTR("%2u write err"), _param);
                Board::msDelay(5000);
                gDisplay.clear();
            }

        } else {
            _param = 1;
        }

        indexChanged = true;
        break;

    default:
        break;
    }

    if (indexChanged) {
        draw();
    }

    return this;
}

void
SetupMode::_init(uint8_t nad, uint8_t max_param)
{
    _nad = nad;
    _max_param = max_param;
}

void
SetupMode::draw()
{
    gDisplay.clear();
    printTitle();
    gDisplay.move(0, 1);
    gDisplay.printf(PSTR(">>"));
    gDisplay.printf((_param == 0) ? 
                    PSTR("Back to node list") : 
                    paramName());
    gDisplay.move(2, 2);

    if (_param > 0) {
        if (!gSlave.getParameter(_nad, _param, _value)) {
            gDisplay.printf(PSTR("read error"));

        } else {
            const char *fmt = paramFormat();
            const char *tab = nullptr;

            if (pgm_read_byte(fmt) == '%') {

                switch (pgm_read_byte(fmt + 1)) {
                case 'O':
                    tab = LIN::strtabRelayID;
                    break;

                case 'S':
                    tab = LIN::strtabSwitchID;
                    break;

                case 'T':
                    tab = LIN::strtabRelayType;
                    break;
                }
            }

            if (tab != nullptr) {
                modeEdit.init(this, &_value, Display::Region(2, 2, 18, 1), tab);
            } else {
                modeEdit.init(this,
                              &_value,
                              Display::Region(2, 2, 18, 1),
                              param().min_value(),
                              param().max_value(), fmt);
            }

            modeEdit.draw();
        }
    }
}

} // namespace Menu
