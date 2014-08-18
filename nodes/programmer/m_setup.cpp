/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include "util.h"
#include "board.h"

#include "m_top.h"
#include "slave.h"

namespace Menu
{

Mode *
ExploreSetupMode::select()
{
    switch (_node) {

    case Master::kNodeAddress:
    case PowerV1::kNodeAddress ... PowerV1::kNodeAddress + 15:
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

Mode *
SetupMode::action(Encoder::Event bp)
{
    bool wantDraw = false;

    if (_editing) {
        switch (bp) {
        case Encoder::kEventDown:

            wantDraw = adjust_value(-1);
            break;

        case Encoder::kEventUp:
            wantDraw = adjust_value(1);
            break;

        case Encoder::kEventPress:
            _editing = false;

            if (!gSlave.set_parameter(_nad, _param.address(), _value)) {
                gDisplay.clear(Display::Region(0, 1, 20, 2));
                gDisplay.move(2, 2);
                gDisplay.printf(PSTR("%2u write err"), _param.address());
                Board::ms_delay(5000);
                gDisplay.clear(Display::Region(0, 1, 20, 2));
                wantDraw = true;
            }

            break;

        case Encoder::kEventActivate:
            wantDraw = true;
            break;

        default:
            break;
        }

    } else {
        bool indexChanged = false;

        switch (bp) {
        case Encoder::kEventDown:
            if (_index > 0) {
                _index--;
                indexChanged = true;
            }

            break;

        case Encoder::kEventUp:
            if (param(_index + 1).exists()) {
                _index++;
                indexChanged = true;
            }

            break;

        case Encoder::kEventPress:
            if (_index == 0) {
                return &modeExploreSetup;
            }

            _editing = true;
            wantDraw = true;
            break;

        case Encoder::kEventActivate:
            gDisplay.clear();
            print_title();
            _index = 1;
            indexChanged = true;
            break;

        default:
            break;
        }

        if (indexChanged) {
            wantDraw = true;
            _param = param(_index);
            _readError = !gSlave.get_parameter(_nad, _param.address(), _value);
        }
    }

    if (wantDraw) {
        draw();
    }

    return this;
}

void
SetupMode::init(uint8_t nad)
{
    uint16_t flavour = 0;

    if (gSlave.get_parameter(nad, 0, flavour)) {
        switch (flavour) {          // XXX kBoardFunctionID value
        // XXX should be using Read By ID
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
    _index = 1;
}

void
SetupMode::draw()
{
    gDisplay.clear(Display::Region(0, 1, 20, 2));

    const char *name = PSTR("Back");

    if (_index > 0) {
        name = param_name(_param.address());
    }

    gDisplay.move(0, 1);
    gDisplay.printf(PSTR("%2s%18s"), _editing ? PSTR("  ") : PSTR(">>"), name);

    if (_index > 0) {

        const char *info = Encoding::info(param_encoding(_param.address()), _value);

        gDisplay.move(0, 2);
        gDisplay.printf(PSTR("%2s"), _editing ? PSTR(">>") : PSTR("  "));

        if (_readError) {
            gDisplay.printf(PSTR("READ ERROR @ 0x%x"), _param.address());

        } else if (info == nullptr) {
            gDisplay.printf(PSTR("%u"), _value);

        } else {
            gDisplay.printf(PSTR("%18s"), info);
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
        msg = PSTR("4ch Power");
        break;

    case kFlavourPowerV3:
        msg = PSTR("5ch Power");
        break;

    default:
        Board::panic(Board::kPanicCodeAssert);
    }

    gDisplay.printf(PSTR("%s Setup:"), msg);
}

uint8_t
SetupMode::param_encoding(Parameter::Address addr)
{
    switch (_flavour) {
    case kFlavourMaster:
        return Master::param_encoding(addr);

    case kFlavourPowerV1:
        return PowerV1::param_encoding(addr);
        break;

    case kFlavourPowerV3:
        return PowerV3::param_encoding(addr);
        break;
    }

    return kEncoding_none;
}

const char *
SetupMode::param_name(Parameter::Address addr)
{
    switch (_flavour) {
    case kFlavourMaster:
        return Master::param_name(addr);

    case kFlavourPowerV1:
        return PowerV1::param_name(addr);
        break;

    case kFlavourPowerV3:
        return PowerV3::param_name(addr);
        break;
    }

    return nullptr;
}

Parameter
SetupMode::param(uint8_t index)
{
    if (index > 0) {

        // scan for the index'th parameter
        for (Parameter::Address addr = 0x0400; addr < 0x4ff; addr++) {

            if (param_encoding(addr) != kEncoding_none) {
                if (--index == 0) {
                    return Parameter(addr);
                }
            }
        }
    }

    // cons up a non-existent parameter
    return Parameter(Parameter::noAddress);
}

bool
SetupMode::adjust_value(int16_t offset)
{
    // check offset doesn't cause wrap
    if ((offset < 0) && ((offset + _value) > _value)) {
        return false;
    }

    if ((offset + _value) < _value) {
        return false;
    }

    uint16_t new_value = _value + offset;
    uint8_t encoding = param_encoding(_param.address());

    if (Encoding::invalid(encoding, new_value)) {
        return false;
    }

    _value = new_value;
    return true;
}

} // namespace Menu
