
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

static PROGMEM const char switchNames[] = 
    "Ignition      \0"
    "Start         \0"
    "MarkerLights  \0"
    "HeadLights    \0"
    "HighBeam      \0"
    "HighBeamToggle\0"
    "FogLight      \0"
    "LeftTurn      \0"
    "RightTurn     \0"
    "Brake         \0"
    "Reverse       \0"
    "Door          \0"
    "InteriorLight \0"
    "Hazard        \0"
    "DoorUnlock    \0"
    "LightsUp      \0"
    "DemoMode      \0"
    "Unassigned    \0"
    "\0";

static PROGMEM const char paramNames[] =
    "TurnBlinkPeriod\0"
    "PassingBlinkCount\0"
    "PathLightPeriod\0"
    "InteriorLightPeriod\0"
    "WelcomeLightPeriod\0"
    "BrakeBlinkPeriod\0"
    "BrakeBlinkCount\0"
    "\0";


void
SetupMasterMode::enter(Mode *from)
{
    gDisplay.clear();

    if (_editing) {
        _editing = false;
        if (!gSlave.setParameter(LIN::kNADMaster, _param, _value)) {
            gDisplay.printf(PSTR("param %2u write error"), _param);
            Board::msDelay(5000);
            gDisplay.clear();
        }
    } else {
        _param = 1;
    }

    draw();
}

Mode *
SetupMasterMode::action(Encoder::Event bp)
{
    bool wantDraw = false;

    switch (bp) {
    case Encoder::kEventDown:
        if (_param > 0) {
            _param--;
        }
        wantDraw = true;
        break;

    case Encoder::kEventUp:
        if (_param < (21 + Util::strtablen(paramNames))) {
            _param++;
        }
        wantDraw = true;
        break;

    case Encoder::kEventPress:
        switch (_param) {
        case 0:
            return &modeExplore;

        case 1 ... 21:
            _editing = true;
            modeEdit.init(&_value, 0, 1, switchNames, PSTR("%s"));
            return &modeEdit;

        default:
            _editing = true;
            modeEdit.init(&_value, 0, 1);
            return &modeEdit;
        }
        break;

    default:
        break;
    }

    if (wantDraw) {
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
        return;
    }

    if (!gSlave.getParameter(LIN::kNADMaster, _param, _value)) {
        gDisplay.printf(PSTR("param %2u read error"), _param);
    } else {
        const char *str;

        switch (_param) {
        case 1 ... 21:
            gDisplay.printf(PSTR("Input %2u"), _param);
            if ((str = Util::strtab(switchNames, _value)) == nullptr) {
                str = PSTR("<unset>");
            }
            gDisplay.move(0, 1);
            gDisplay.printf(str);
            break;

        default:
            gDisplay.printf(PSTR("%s"), Util::strtab(paramNames, _param - 22));
            gDisplay.move(0, 1);
            gDisplay.printf(PSTR("%3u"), _value);
            break;
        }
    }
}

} // namespace Menu
