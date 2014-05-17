
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
            return &modeEdit;
        }
        break;

    case Encoder::kEventActivate:
#ifdef DEBUG
        if (Util::strtablen(relayNames) != (LIN::kRelayMax + 1)) {
            gDisplay.clear();
            debug("Menu::relayNames %u out of sync with LIN::RelayID %u", Util::strtablen(relayNames), LIN::kRelayMax + 1);
            Board::panic(Board::kPanicAssert);
        }
#endif
        if (_editing) {
            _editing = false;
            if (!gSlave.setParameter(_node, _param, _value)) {
                gDisplay.printf(PSTR("%2u write err"), _param);
                Board::msDelay(5000);
                gDisplay.clear();
            }
        } else {
            _param = 1;
        }
        wantDraw = true;
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
        gDisplay.printf(PSTR("%2u read err"), _param);
        return;
    }
    gDisplay.printf(PSTR("Relay %2u"), _param);
    modeEdit.init(this, &_value, 0, 1, relayNames, PSTR("%16s"));
    modeEdit.draw();
}

} // namespace Menu
