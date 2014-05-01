
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
    Master      master;         // XXX must be before switches to get !SS as an output 
    Switches    switches;       // XXX should move LINCS somewhere else, use PA6 for SPI !SS

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
        if (switches[LIN::kSWDoor] || switches[LIN::kSWInteriorLight]) {
            // XXX stay-on timer goes here
            f.set(LIN::kRelayInteriorLight);
            // XXX path lighting if ignition was just turned off
        }
        if (switches[LIN::kSWHazard]) {
            // XXX test blink timer
            f.set(LIN::kRelayLeftTurn);
            f.set(LIN::kRelayRightTurn);
        }

        // relays only on with ignition
        if (switches[LIN::kSWIgnition]) {

            // high/low-beam lights
            if (switches[LIN::kSWHighBeam]) {
                f.set(LIN::kRelayHighBeam);
            } else if (switches[LIN::kSWLowBeam]) {
                f.set(LIN::kRelayLowBeam);
            }

            // markers and city lights
            if (switches[LIN::kSWLights] || 
                switches[LIN::kSWLowBeam] ||
                switches[LIN::kSWHighBeam]) {
                f.set(LIN::kRelayMarkers);
                f.set(LIN::kRelayCityLights);
            }

            // foglights
            // XXX require other lights?
            if (switches[LIN::kSWFogLight]) {
                f.set(LIN::kRelayFogLights);
            }

            // popup headlights 
            // up when headlights go on, down when all lights go off
            if (switches[LIN::kSWLowBeam] ||
                switches[LIN::kSWHighBeam]) {
                f.set(LIN::kRelayLightsUp);
                // minimum raise signal time?
            } else if (!switches[LIN::kSWLights]) {
                f.set(LIN::kRelayLightsDown);
                // minimum lower signal time?
            }

            // turn signals
            // XXX test keep-blinking timer
            if (switches[LIN::kSWLeftTurn]) {
                // XXX test blink timer
                f.set(LIN::kRelayLeftTurn);
            }
            if (switches[LIN::kSWRightTurn]) {
                // XXX test blink timer
                f.set(LIN::kRelayRightTurn);
            }

            // brake lights
            // XXX test minimum-brake timer
            // XXX test brake-blnk timer
            if (switches[LIN::kSWBrake]) {
                f.set(LIN::kRelayBrake);
            }

            // reverse lights
            if (switches[LIN::kSWReverse]) {
                f.set(LIN::kRelayReverse);
            }

            // relays only on without ignition
        } else {

            // parking lights - no blink
            if (switches[LIN::kSWLeftTurn]) {
                f.set(LIN::kRelayLeftTurn);
            }
            if (switches[LIN::kSWRightTurn]) {
                f.set(LIN::kRelayRightTurn);
            }
        }

        // update the copy we are sending to nodes
        master.relayFrame.copy(f);
    }
}

