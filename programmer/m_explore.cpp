#include <avr/wdt.h>

#include "lin_protocol.h"
#include "board.h"

#include "m_explore.h"
#include "m_top.h"
#include "m_setup_master.h"
#include "m_setup_power.h"
#include "slave.h"

namespace Menu
{

ExploreMode modeExplore;

////////////////////////////////////////////////////////////////////////////////
// Explore mode
//

void
ExploreMode::enter(Mode *from)
{
    gDisplay.clear();
    if (!nodePresent(LIN::kNADMaster)) {
        gDisplay.clear();
        gDisplay.printf(PSTR("Master node not found"));
        Board::msDelay(5000);
        _node = 0;
    } else {
        _node = LIN::kNADMaster;
    }
    draw();
}

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
        case LIN::kNADPowerBase ... (LIN::kNADPowerBase + 15):
            modeSetupPower.init(_node);
            return &modeSetupPower;
        default:
            break;
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
    for (uint8_t newNode = _node + 1; newNode < LIN::kNADMaxAssigned; newNode ++) {
        wdt_reset();
        if (nodePresent(newNode)) {
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
        if (nodePresent(newNode)) {
            break;
        }
    }
    _node = newNode;
    return true;
}

bool
ExploreMode::nodePresent(uint8_t node)
{
    uint8_t dummy;

    gDisplay.clear();
    gDisplay.printf(PSTR("scan %2u"), node);

    return gSlave.getParameter(node, 0, dummy);
}

} // namespace Menu
