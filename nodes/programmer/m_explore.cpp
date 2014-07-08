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
// +--------------------+
// |                    |
// | Searching... XX    |
// |                    |
// |                    |
// +--------------------+
//
// +--------------------+
// |Error:              |
// | Master node not    |
// | responding.        |
// |                    |
// +--------------------+
//
// +--------------------+
// |Select:             |
// |    -cancel-        |
// | >>Master           |
// |   Power #4         |
// +--------------------+
//
Mode *
ExploreMode::action(Encoder::Event bp)
{
    bool wantDraw = false;

    switch (bp) {

    case Encoder::kEventUp:
        _node = searchDown(_node);
        Encoder::discard();
        wantDraw = true;
        break;

    case Encoder::kEventDown:
        _node = searchUp(_node);
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
        presentMask.set((LIN::NodeAddress)0);   // used for the 'cancel' entry

        for (uint8_t i = LIN::kNADMaster; i < LIN::kNADMaxAssigned; i++) {
            gDisplay.clear();
            gDisplay.move(1, 1);
            gDisplay.printf(PSTR("Searching... %2u"), i);

            uint8_t dummy;

            if (gSlave.getParameter(i, 0, dummy)) {
                presentMask.set(i);
            }
        }

        if (!presentMask.test(LIN::kNADMaster)) {
            error(PSTR(" Master node not\n responding."));
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
    uint8_t base;       // base for the list to be drawn
    uint8_t cursor;     // offset row for the cursor

    // To give the illusion of scrolling in a list, we need to look around the current
    // node a bit.
    if (searchDown(_node) == _node) {
        base = _node;   // nothing earlier in the list
        cursor = 1;

    } else if ((searchUp(_node) == _node) &&
               (searchDown(_node) != searchDown(searchDown(_node)))) {
        base = searchDown(searchDown(_node));
        cursor = 3;

    } else {
        base = searchDown(_node);
        cursor = 2;
    }

    gDisplay.clear();
    gDisplay.printf(PSTR("Select:"));

    for (uint8_t row = 1; row < 4; row++) {
        gDisplay.move(3, row);

        switch (base) {
        case 0:
            gDisplay.printf(PSTR(" -cancel-"));
            break;

        case 1:
            gDisplay.printf(PSTR("Master"));
            break;

        case LIN::kNADPowerBase ... LIN::kNADPowerBase+ 16:
            gDisplay.printf(PSTR("Power %2u"), base - LIN::kNADPowerBase);
            break;

        default:
            gDisplay.printf(PSTR("Node @ %2u"), base);
            break;
        }

        uint8_t next = searchUp(base);

        if (next == base) {
            break;
        }

        base = next;
    }

    gDisplay.move(1, cursor);
    gDisplay.printf(PSTR(">>"));
}

uint8_t
ExploreMode::searchUp(uint8_t from)
{
    for (uint8_t newNode = from + 1; newNode < LIN::kNADMaxAssigned; newNode++) {
        if (presentMask.test(newNode)) {
            return newNode;
        }
    }

    return from;
}

uint8_t
ExploreMode::searchDown(uint8_t from)
{
    if (from > 0) {
        uint8_t newNode = from - 1;

        do {
            if (presentMask.test(newNode)) {
                return newNode;
            }
        } while (newNode-- >= 0);
    }

    return from;
}

} // namespace Menu
