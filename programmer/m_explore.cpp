#include <avr/wdt.h>

#include "lin_protocol.h"
#include "board.h"

#include "m_explore.h"
#include "m_top.h"
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
        wantDraw = true;
        break;

    case Encoder::kEventUp:
        searchUp();
        wantDraw = true;
        break;

    case Encoder::kEventPress:
        if (_node == 0) {
            return &modeTop;
        } else {
            // XXX configure node
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
    if (_node == 0) {
        gDisplay.printf(PSTR("Main Menu"));
    } else {
        gDisplay.printf(PSTR("Node %2u"), _node);
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
