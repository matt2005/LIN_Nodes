
#include "util.h"
#include "board.h"

#include "lin_protocol.h"
#include "protocol.h"

#include "m_top.h"
#include "slave.h"

namespace Menu
{

Mode *
ExploreSetupMode::select()
{
    switch (_node) {

    case LIN::kNodeAddressMaster:
        modeSetupMaster.init();
        return &modeSetupMaster;

    case LIN::kNodeAddressPowerBase ...(LIN::kNodeAddressPowerBase + 15):
        modeSetupPower.init(_node);
        return &modeSetupPower;

    default:
        break;
    }

    return nullptr;
}

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

uint8_t  SetupMode::_param;
uint8_t  SetupMode::_value;
uint8_t  SetupMode::_nad;
uint8_t  SetupMode::_max_param;
bool     SetupMode::_editing;

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
            return &modeExploreSetup;

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

            if (!gSlave.set_parameter(_nad, _param, _value)) {
                gDisplay.clear();
                gDisplay.printf(PSTR("%2u write err"), _param);
                Board::ms_delay(5000);
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
    print_title();
    gDisplay.move(0, 1);
    gDisplay.printf(PSTR(">>"));
    gDisplay.printf((_param == 0) ?
                    PSTR("Back to node list") :
                    param_name());
    gDisplay.move(2, 2);

    if (_param > 0) {
        if (!gSlave.get_parameter(_nad, _param, _value)) {
            gDisplay.printf(PSTR("read error"));

        } else {
            const char *fmt = param_format();
            const char *tab = nullptr;

            if (pgm_read_byte(fmt) == '%') {

                switch (pgm_read_byte(fmt + 1)) {
                case 'O':
                    tab = LIN::namesForRelayID;
                    break;

                case 'S':
                    tab = LIN::namesForSwitchID;
                    break;

                case 'T':
                    tab = LIN::namesForRelayType;
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
