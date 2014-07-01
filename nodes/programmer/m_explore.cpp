#include <avr/wdt.h>

#include "lin_protocol.h"
#include "board.h"
#include "util.h"

#include "m_explore.h"
#include "m_top.h"
#include "m_setup_master.h"
#include "m_setup_power.h"
#include "slave.h"

namespace Menu
{

ExploreMode modeExplore;

Util::Bitarray<LIN::kNADMaxAssigned> presentMask;

////////////////////////////////////////////////////////////////////////////////
// Explore mode
//

Mode *
ExploreMode::action(Encoder::Event bp)
{
    bool wantDraw = false;

    switch (bp) {

    case Encoder::kEventDown:
        searchDown();
        Encoder::discard();
        wantDraw = true;
        break;

    case Encoder::kEventUp:
        searchUp();
        Encoder::discard();
        wantDraw = true;
        break;

    case Encoder::kEventPress:
        switch (_node) {
        case 0:
            return &modeTop;

        case LIN::kNADMaster:
            return &modeSetupMaster;

        case LIN::kNADPowerBase ...(LIN::kNADPowerBase + 15):
            modeSetupPower.init(_node);
            return &modeSetupPower;

        default:
            break;
        }

        break;

    case Encoder::kEventActivate:
        gDisplay.clear();
        presentMask.reset();

        for (uint8_t i = LIN::kNADMaster; i < LIN::kNADMaxAssigned; i++) {
            gDisplay.clear();
            gDisplay.printf(PSTR("Scan...%2u"), i);

            uint8_t dummy;

            if (gSlave.getParameter(i, 0, dummy)) {
                presentMask.set(i);
            }
        }

        if (!presentMask.test(LIN::kNADMaster)) {
            gDisplay.clear();
            gDisplay.printf(PSTR("Master node not found"));
            Board::msDelay(3000);
            _node = 0;

        } else {
            _node = LIN::kNADMaster;
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
ExploreMode::draw()
{
    gDisplay.clear();

    switch (_node) {
    case 0:
        gDisplay.printf(PSTR(">back"));
        break;

    case 1:
        gDisplay.printf(PSTR("Master Node"));
        break;

    case 2 ... 17:
        gDisplay.printf(PSTR("Power Node %2u"), _node - 1);
        break;

    default:
        gDisplay.printf(PSTR("Node %2u"), _node);
        break;
    }
}

bool
ExploreMode::searchUp()
{
    for (uint8_t newNode = _node + 1; newNode < LIN::kNADMaxAssigned; newNode++) {
        if (presentMask.test(newNode)) {
            _node = newNode;
            return true;
        }
    }

    return false;
}

bool
ExploreMode::searchDown()
{
    if (_node == 0) {
        return false;
    }

    uint8_t newNode;

    for (newNode = _node - 1; newNode > 0; newNode--) {
        wdt_reset();

        if (presentMask.test(newNode)) {
            break;
        }
    }

    _node = newNode;
    return true;
}

} // namespace Menu
