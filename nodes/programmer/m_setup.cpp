
#include "util.h"
#include "board.h"
#include "param_master.h"
#include "param_power_v1.h"
#include "param_power_v3.h"

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
    case LIN::kNodeAddressPowerBase ...(LIN::kNodeAddressPowerBase + 15):
        modeSetup.init(_node);
        return &modeSetup;

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

uint8_t         SetupMode::_nad;
uint8_t         SetupMode::_param;
uint8_t         SetupMode::_value;
SetupMode::Flavour SetupMode::_flavour;
uint8_t         SetupMode::_max_param;
bool            SetupMode::_editing;

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
SetupMode::init(uint8_t nad)
{
    uint8_t flavour = 0;

    if (gSlave.get_parameter(nad, 0, flavour)) {
        switch (flavour) {          // XXX kBoardFunctionID value
        case 0:
            _flavour = kFlavourMaster;
            break;

        case 1:
            _flavour = kFlavourPowerV1;
            break;

        case 2:
            _flavour = kFlavourPowerV3;
            break;

        default:
            // XXX should retry / bail out
            Board::panic(Board::kPanicCodeAssert);
        }
    }

    _nad = nad;
    _max_param = Util::strtablen(param_names()) - 1;
}

void
SetupMode::draw()
{
    gDisplay.clear();
    print_title();
    gDisplay.move(0, 1);
    gDisplay.printf(PSTR(">>"));
    gDisplay.printf((_param == 0) ?
                    PSTR("Back") :
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
                    tab = namesForRelayID;
                    break;

                case 'S':
                    tab = namesForSwitchID;
                    break;

                case 'T':
                    tab = namesForRelayType;
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

void
SetupMode::print_title()
{
    const char *msg = PSTR("unknown");

    switch (_flavour) {
    case kFlavourMaster:
        msg = PSTR("Master");
        break;

    case kFlavourPowerV1:
    case kFlavourPowerV3:
        msg = PSTR("Power Node");
        break;

    default:
        Board::panic(Board::kPanicCodeAssert);
    }

    gDisplay.printf(PSTR("%s Setup:"), msg);
}

Parameter
SetupMode::param()
{
    switch (_flavour) {
    case kFlavourMaster:
        return masterParam(_param);

    case kFlavourPowerV1:
        return power_v1Param(_param);

    case kFlavourPowerV3:
        return power_v3Param(_param);

    default:
        Board::panic(Board::kPanicCodeAssert);
    }
}

PGM_P
SetupMode::param_names()
{
    switch (_flavour) {
    case kFlavourMaster:
        return masterParamNames;

    case kFlavourPowerV1:
        return power_v1ParamNames;

    case kFlavourPowerV3:
        return power_v3ParamNames;

    default:
        Board::panic(Board::kPanicCodeAssert);
    }
}

PGM_P
SetupMode::param_formats()
{
    switch (_flavour) {
    case kFlavourMaster:
        return masterParamFormats;

    case kFlavourPowerV1:
        return power_v1ParamFormats;

    case kFlavourPowerV3:
        return power_v3ParamFormats;

    default:
        Board::panic(Board::kPanicCodeAssert);
    }
}


} // namespace Menu
