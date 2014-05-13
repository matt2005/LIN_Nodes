
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

static PROGMEM const char paramInfo[] =
    "TurnBlinkPeriod\0"     "%3u0ms\0"
    "PassingBlinkCount\0"   "%3u blinks\0"
    "PathLightPeriod\0"     "%3us\0"
    "InteriorLightPeriod\0" "%3us\0"
    "WelcomeLightPeriod\0"  "%3us\0"
    "BrakeBlinkPeriod\0"    "%3u0ms\0"
    "BrakeBlinkCount\0"     "%3u blinks\0"
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
    bool indexChanged = false;

    switch (bp) {
    case Encoder::kEventDown:
        if (_param > 0) {
            _param--;
        }
        indexChanged = true;
        break;

    case Encoder::kEventUp:
        if (pgm_read_byte(paramName(_param + 1)) != '\0') {
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
    switch (_param) {
    case 0:
        gDisplay.printf(PSTR(">back"));
        break;

    case 1 ... 22:      // paramSGAssign(14).index()
        gDisplay.printf(PSTR("Input %2u"), _param);
        modeEdit.init(&_value, 0, 1, switchNames, PSTR("%s"));
        modeEdit.draw();
        break;

    default:
        gDisplay.printf(PSTR("%s"), paramName(_param));
        modeEdit.init(&_value, 0, 1, 0, 255, paramFormat(_param));
        modeEdit.draw();
        break;
    }
}

} // namespace Menu
