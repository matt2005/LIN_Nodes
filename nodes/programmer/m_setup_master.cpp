
#include "lin_protocol.h"
#include "util.h"
#include "board.h"

#include "m_setup_master.h"
#include "m_explore.h"
#include "m_edit.h"
#include "slave.h"

namespace Menu
{

SetupMasterMode modeSetupMaster;

static PROGMEM const char paramInfo[] =
    " \0"                   " \0"
    "SP1\0"                 " \0"
    "SP2\0"                 " \0"
    "SP3\0"                 " \0"
    "SP4\0"                 " \0"
    "SP5\0"                 " \0"
    "SP6\0"                 " \0"
    "SP7\0"                 " \0"
    "SG1\0"                 " \0"
    "SG2\0"                 " \0"
    "SG3\0"                 " \0"
    "SG4\0"                 " \0"
    "SG5\0"                 " \0"
    "SG6\0"                 " \0"
    "SG7\0"                 " \0"
    "SG8\0"                 " \0"
    "SG9\0"                 " \0"
    "SG10\0"                " \0"
    "SG11\0"                " \0"
    "SG12\0"                " \0"
    "SG13\0"                " \0"
    "SG14\0"                " \0"
    ////////////////
    "TurnBlinkPeriod \0"    "%3u0ms\0"
    "PassBlinkCount  \0"    "%3u blinks\0"
    "PathLightTime   \0"    "%3us\0"
    "InsideLightTime \0"    "%3us\0"
    "WelcomeLightTime\0"    "%3us\0"
    "BrakeBlinkPeriod\0"    "%3u0ms\0"
    "BrakeBlinkCount \0"    "%3u blinks\0"
    "WiperInterval   \0"    "%3u00ms\0"
    "\0";

static const char *
paramName(uint8_t index)
{
    return Util::strtab(paramInfo, index * 2);
}

static const char *
paramFormat(uint8_t index)
{
    return Util::strtab(paramInfo, index * 2 + 1);
}

Mode *
SetupMasterMode::action(Encoder::Event bp)
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
        if (paramName(_param + 2) != nullptr) {
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

    if (_param == 0) {
        gDisplay.printf(PSTR(">back"));

    } else {
        if (!gSlave.getParameter(LIN::kNADMaster, _param, _value)) {
            gDisplay.printf(PSTR("param %2u read error"), _param);

        } else {
            gDisplay.printf(PSTR("%s"), paramName(_param));

            if (pgm_read_byte(paramFormat(_param)) == ' ') {
                modeEdit.init(this, &_value, 0, 1, switchNames, PSTR("%16s"));

            } else {
                modeEdit.init(this, &_value, 0, 1, 0, 255, paramFormat(_param));
            }

            modeEdit.draw();
        }
    }
}

} // namespace Menu
