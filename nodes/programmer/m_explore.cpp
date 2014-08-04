/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <avr/wdt.h>

#include "lin_protocol.h"
#include "board.h"
#include "util.h"

#include "m_top.h"
#include "slave.h"

namespace Menu
{

StaticBitarray<LIN::kNodeAddressMaxAssigned> ExploreMode::_presentMask;
uint8_t                                ExploreMode::_node;

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
        _node = search_down(_node);
        Encoder::discard();
        wantDraw = true;
        break;

    case Encoder::kEventDown:
        _node = search_up(_node);
        Encoder::discard();
        wantDraw = true;
        break;

    case Encoder::kEventPress:
        switch (_node) {
        case 0:
            // force rescan on re-entry from top menu
            _presentMask.reset();
            return &modeTop;

        default: {
            Mode *newmode = select();

            if (newmode != nullptr) {
                return newmode;
            }
        }
        break;
        }

        break;

    case Encoder::kEventActivate:
        gDisplay.clear();

        // do we have a valid scan result with a master node?
        // (i.e. are we re-entering from an edit state?)
        if (!_presentMask.test(LIN::kNodeAddressMaster)) {
            _presentMask.reset();
            _presentMask.set((uint8_t)0);   // used for the 'cancel' entry
            _node = LIN::kNodeAddressMaster;

            for (uint8_t i = LIN::kNodeAddressMaster; i < LIN::kNodeAddressMaxAssigned; i++) {
                gDisplay.clear();
                gDisplay.move(1, 1);
                gDisplay.printf(PSTR("Searching... %2u"), i);

                uint8_t dummy;

                if (gSlave.get_parameter(i, 0, dummy)) {
                    _presentMask.set(i);

                } else if (i == LIN::kNodeAddressMaster) {
                    error(PSTR(" Master node not\n responding."));
                    Board::ms_delay(3000);
                    _node = 0;
                    break;
                }
            }
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

Mode *
ExploreMode::select()
{
    return nullptr;
}

void
ExploreMode::draw()
{
    uint8_t base;       // base for the list to be drawn
    uint8_t cursor;     // offset row for the cursor

    // To give the illusion of scrolling in a list, we need to look around the current
    // node a bit.
    if (search_down(_node) == _node) {
        base = _node;   // nothing earlier in the list
        cursor = 1;

    } else if ((search_up(_node) == _node) &&
               (search_down(_node) != search_down(search_down(_node)))) {
        base = search_down(search_down(_node));
        cursor = 3;

    } else {
        base = search_down(_node);
        cursor = 2;
    }

    gDisplay.clear();
    gDisplay.printf(PSTR("Select:"));

    for (uint8_t row = 1; row < 4; row++) {
        gDisplay.move(3, row);

        switch (base) {
        case 0:
            gDisplay.printf(PSTR("Back"));
            break;

        case 1:
            gDisplay.printf(PSTR("Master"));
            break;

        case LIN::kNodeAddressPowerBase ... LIN::kNodeAddressPowerBase+ 16:
            gDisplay.printf(PSTR("Power %2u"), base - LIN::kNodeAddressPowerBase);
            break;

        default:
            gDisplay.printf(PSTR("Node @ %2u"), base);
            break;
        }

        uint8_t next = search_up(base);

        if (next == base) {
            break;
        }

        base = next;
    }

    gDisplay.move(1, cursor);
    gDisplay.printf(PSTR(">>"));
}

uint8_t
ExploreMode::search_up(uint8_t from)
{
    for (uint8_t newNode = from + 1; newNode < LIN::kNodeAddressMaxAssigned; newNode++) {
        if (_presentMask.test(newNode)) {
            return newNode;
        }
    }

    return from;
}

uint8_t
ExploreMode::search_down(uint8_t from)
{
    if (from > 0) {
        uint8_t newNode = from - 1;

        do {
            if (_presentMask.test(newNode)) {
                return newNode;
            }
        } while (newNode-- >= 0);
    }

    return from;
}

} // namespace Menu
