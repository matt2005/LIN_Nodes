
#include "util.h"
#include "board.h"

#include "lin_protocol.h"
#include "protocol.h"
#include "param_master.h"

#include "m_setup_master.h"
#include "m_explore.h"
#include "m_edit.h"
#include "slave.h"

namespace Menu
{

////////////////////////////////////////////////////////////////
// Master node parameter editor
//
// +--------------------+
// |Master:             |
// | >>ParameterName    |
// |    ValueUnits      |
// |                    |
// +--------------------+
//
// +--------------------+
// |Master:             |
// | >> -done-          |
// |                    |
// |                    |
// +--------------------+
//

SetupMasterMode modeSetupMaster;

static const char *
paramName(uint8_t index)
{
    if (index == 0) {
        return PSTR(" -done- ");

    } else {
        return Util::strtab(masterParamNames, index);
    }
}

static const char *
paramFormat(uint8_t index)
{
    if (index == 0) {
        return PSTR("");

    } else {
        return Util::strtab(masterParamFormats, index);
    }
}

Mode *
SetupMasterMode::action(Encoder::Event bp)
{
    bool indexChanged = false;

    switch (bp) {
    case Encoder::kEventUp:
        if (_param > 0) {
            _param--;
        }

        indexChanged = true;
        break;

    case Encoder::kEventDown:
        if (_param < (kParamMax - 1)) {
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
            return &modeEdit;
        }

        break;

    case Encoder::kEventActivate:
        if (_editing) {
            _editing = false;

            if (!gSlave.setParameter(LIN::kNADMaster, _param, _value)) {
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
SetupMasterMode::draw()
{
    gDisplay.clear();
    gDisplay.printf(PSTR("Master:"));
    gDisplay.move(1, 1);
    gDisplay.printf(paramName(_param));
    gDisplay.move(2, 2);

    if (_param > 0) {
        if (!gSlave.getParameter(LIN::kNADMaster, _param, _value)) {
            gDisplay.printf(PSTR("read error"));

        } else {
            const char *fmt = paramFormat(_param);
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
                modeEdit.init(this, &_value, 3, 2, tab);
            } else {
                modeEdit.init(this,
                              &_value,
                              3,
                              2,
                              masterParam(_param).min_value(),
                              masterParam(_param).max_value(), fmt);
            }
            modeEdit.draw();

//            gDisplay.printf(paramFormat(_param), _value);
//
//            gDisplay.printf(PSTR("%s"), paramName(_param));
//
//            if (pgm_read_byte(paramFormat(_param)) == ' ') {
//                modeEdit.init(this, &_value, 0, 1, LIN::strtabSwitchID, PSTR("%16s"));
//
//            } else {
//                modeEdit.init(this, &_value, 0, 1, 0, 255, paramFormat(_param));
//            }
//
//            modeEdit.draw();
        }
    }
}

} // namespace Menu
