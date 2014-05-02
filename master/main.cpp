
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "board.h"
#include "lin_protocol.h"

#include "display.h"
#include "master.h"
#include "menu.h"
#include "switches.h"

void
main(void)
{
    Board       board;

    // check for recovery mode before constructing anything else
    if (Board::getMode() != 0) {
        Board::panic(Board::kPanicRecovery);
    }

    // construct drivers that we need
    Master      master;
    Switches    switches;

    // enable interrupts; timers and LIN events will start.
    sei();

    // check for an attached display, run setup mode if attached
    Display     disp;
    if (disp.probe()) {
        Menu        menu(disp, master);

        debug("Display found, entering setup mode");

        // run the menu state machine forever
        for (;;) {
            wdt_reset();
            menu.tick();
            switches.scan();
        }
    }

    // run the master logic forever
    for (;;) {
        wdt_reset();
        switches.scan();

        // update the relays frame by looking at switches
        LIN::RelayFrame f;

        // interior light
        if (switches.test(LIN::kSWDoor) || switches.test(LIN::kSWInteriorLight)) {
            // XXX stay-on timer goes here
            f.set(LIN::kRelayInteriorLight);
            // XXX path lighting if ignition was just turned off
        }
        if (switches.test(LIN::kSWHazard)) {
            // XXX test blink timer
            f.set(LIN::kRelayLeftTurn);
            f.set(LIN::kRelayRightTurn);
        }

        // relays only on with ignition
        if (switches.test(LIN::kSWIgnition)) {

            // high/low-beam lights
            if (switches.test(LIN::kSWHighBeam)) {
                f.set(LIN::kRelayHighBeam);
            } else if (switches.test(LIN::kSWLowBeam)) {
                f.set(LIN::kRelayLowBeam);
            }

            // markers and city lights
            if (switches.test(LIN::kSWLights) || 
                switches.test(LIN::kSWLowBeam) ||
                switches.test(LIN::kSWHighBeam)) {
                f.set(LIN::kRelayMarkers);
                f.set(LIN::kRelayCityLights);
            }

            // foglights
            // XXX require other lights?
            if (switches.test(LIN::kSWFogLight)) {
                f.set(LIN::kRelayFogLights);
            }

            // popup headlights 
            // up when headlights go on, down when all lights go off
            if (switches.test(LIN::kSWLowBeam) ||
                switches.test(LIN::kSWHighBeam)) {
                f.set(LIN::kRelayLightsUp);
                // minimum raise signal time?
            } else if (!switches.test(LIN::kSWLights)) {
                f.set(LIN::kRelayLightsDown);
                // minimum lower signal time?
            }

            // turn signals
            // XXX test keep-blinking timer
            if (switches.test(LIN::kSWLeftTurn)) {
                // XXX test blink timer
                f.set(LIN::kRelayLeftTurn);
            }
            if (switches.test(LIN::kSWRightTurn)) {
                // XXX test blink timer
                f.set(LIN::kRelayRightTurn);
            }

            // brake lights
            // XXX test minimum-brake timer
            // XXX test brake-blnk timer
            if (switches.test(LIN::kSWBrake)) {
                f.set(LIN::kRelayBrake);
            }

            // reverse lights
            if (switches.test(LIN::kSWReverse)) {
                f.set(LIN::kRelayReverse);
            }

            // relays only on without ignition
        } else {

            // parking lights - no blink
            if (switches.test(LIN::kSWLeftTurn)) {
                f.set(LIN::kRelayLeftTurn);
            }
            if (switches.test(LIN::kSWRightTurn)) {
                f.set(LIN::kRelayRightTurn);
            }
        }

        // update the copy we are sending to nodes
        master.relayFrame.copy(f);
    }
}

