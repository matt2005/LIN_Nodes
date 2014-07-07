
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

        // XXX 4 for v1, 5 for v2, need to identify (or ignore on v1?)
        if (_param < 5) {
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
    modeEdit.init(this, &_value, 0, 1, LIN::strtabRelayID, PSTR("%16s"));
    modeEdit.draw();
}

} // namespace Menu
