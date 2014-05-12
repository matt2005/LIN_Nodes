
#include "lin_protocol.h"
#include "util.h"
#include "board.h"

#include "m_setup_power.h"
#include "m_explore.h"
#include "m_edit.h"
#include "slave.h"

namespace Menu
{

SetupPowerMode modeSetupPower;

static PROGMEM const char relayNames[] = 
    "Ignition       \0"
    "Start          \0"
    "LightsUp       \0"
    "LightsDown     \0"
    "HeadLights     \0"
    "LowBeam        \0"
    "HighBeam       \0"
    "FogLights      \0"
    "Markers        \0"
    "LeftTurn       \0"
    "LeftTurnMarker \0"
    "RightTurn      \0"
    "RightTurnMarker\0"
    "Brake          \0"
    "Reverse        \0"
    "InteriorLight  \0"
    "Unassigned     \0"
    "\0";


void
SetupPowerMode::enter(Mode *from)
{
    gDisplay.clear();

#ifdef DEBUG
    if (Util::strtablen(relayNames) != (LIN::kRelayMax + 1)) {
        debug("Menu::relayNames %u out of sync with LIN::RelayID %u", Util::strtablen(relayNames), LIN::kRelayMax + 1);
        Board::panic(Board::kPanicAssert);
    }
#endif

    if (_editing) {
        _editing = false;
        if (!gSlave.setParameter(_node, _param, _value)) {
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
SetupPowerMode::action(Encoder::Event bp)
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
        if (_param < 4) {
            _param++;
        }
        wantDraw = true;
        break;

    case Encoder::kEventPress:
        switch (_param) {
        case 0:
            return &modeExplore;

        default:
            _editing = true;
            modeEdit.init(&_value, 0, 1, relayNames, PSTR("%s"));
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
SetupPowerMode::draw()
{
    gDisplay.clear();

    if (_param == 0) {
        gDisplay.printf(PSTR(">back"));
        return;
    }

    if (!gSlave.getParameter(_node, _param, _value)) {
        gDisplay.printf(PSTR("param %2u read error"), _param);
    } else {
        const char *str;

        gDisplay.printf(PSTR("Relay %2u"), _param);
        if ((str = Util::strtab(relayNames, _value)) == nullptr) {
            str = PSTR("<unset>");
        }
        gDisplay.move(0, 1);
        gDisplay.printf(str);
    }
}

} // namespace Menu
