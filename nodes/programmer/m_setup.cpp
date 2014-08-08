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

uint8_t         SetupMode::_nad;
uint8_t         SetupMode::_index;
uint16_t        SetupMode::_value;
SetupMode::Flavour SetupMode::_flavour;
bool            SetupMode::_editing;
bool            SetupMode::_readError;

Mode *
SetupMode::action(Encoder::Event bp)
{
    bool wantDraw = false;

    if (!_editing) {
        switch (bp) {
        case Encoder::kEventDown:
            wantDraw = param(_index).next(_value, -1);
            break;

        case Encoder::kEventUp:
            wantDraw = param(_index).next(_value, 1);
            break;

        case Encoder::kEventPress:
            _editing = false;

            if (!gSlave.set_parameter(_nad, param(_index).address(), _value)) {
                gDisplay.clear(Display::region(0, 1, 20, 2));
                gDisplay.move(2, 2);
                gDisplay.printf(PSTR("%2u write err"), param(_index).address());
                Board::ms_delay(5000);
                gDisplay.clear(Display::region(0, 1, 20, 2));
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
                uint8_t new_index = _index - 1;
                do {
                    if (param(new_index).exists()) {
                        _index = new_index;
                        indexChanged = true;
                        break;
                    }
                } while (_index-- > 0);
            }
            break;

        case Encoder::kEventUp:
            for (uint8_t new_index = _index + 1; new_index < 0xfe; new_index++) {
                if (param(new_index).exists()) {
                    _index = new_index;
                    indexChanged = true;
                    break;
                }
            }
            break;

        case Encoder::kEventPress:
            if (index == 0) {
                return &modeExploreSetup;
            }
            _editing = true;
            wantDraw = true;
            break;

        case Encoder::kEventActivate:
            gDisplay.clear();
            print_title();

                modeEdit.init(this, Display::Region(2, 2, 18, 1), _value);
            }

            _index = 1;
            indexChanged = true;
            break;

        default:
            break;
        }

        if (indexChanged) {
            wantDraw = true;

            _readError = !gSlave.get_parameter(_nad, param(_index).address(), _value));
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
    _index = 1;
}

void
SetupMode::draw()
{
    gDisplay.clear(Display::region(0, 1, 20, 2));

    if (_index == 0) {}
        gDisplay.move(0, 1);
        gDisplay.printf(PSTR(">>Back"));
    } else {

        const char *info = param(_index).info(_value);

        gDisplay.move(2, 1);
        gDisplay.printf(PSTR("%2s%18s"), _editing ? PSTR("  ") : PSTR(">>"), param(index).name());

        gDisplay.move(2, 2);
        gDisplay.printf(PSTR("%2s"), _editing ? PSTR(">>") : PSTR("  "));
        if (_readError) {
            gDisplay.printf(PSTR("READ ERROR @ 0x%x"), p.address());
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

Parameter
SetupMode::param(uint8_t index)
{
    // cons up a non-existent parameter
    if (index == 0) {
        return Generic::parameter(0xffff);
    }

    switch (_flavour) {
    case kFlavourMaster:
        return Master::parameter(Generic::ConfigBase + index - 1);

    case kFlavourPowerV1:
        return PowerV1::parameter(Generic::ConfigBase + index - 1);

    case kFlavourPowerV3:
        return PowerV3::parameter(Generic::ConfigBase + index - 1);

    default:
        Board::panic(Board::kPanicCodeAssert);
    }
}

