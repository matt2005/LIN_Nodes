
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "board.h"
#include "lin_protocol.h"
#include "timer.h"

#include "display.h"
#include "master.h"
#include "menu.h"
#include "switches.h"

bool __cxa_guard_acquire() { return true; }
void __cxa_guard_release() {}

class TurnBlinker : public Timer
{
public:
    TurnBlinker() :
        Timer(blink, this),
        _state(true)
    {}
    bool        state() const { return _state; }
    void        reset() {
        _state = true;
        _remaining = _interval = paramTurnBlinkPeriod.get() * 100;
    }

private:
    volatile bool        _state;

    static void blink(void *arg) {
        auto b = reinterpret_cast<TurnBlinker *>(arg);
        b->_state = !b->_state;
    }
};

class BrakeBlinker : public Timer
{
public:
    BrakeBlinker() :
        Timer(blink, this),
        _state(true),
        _count(0)
    {}

    bool        state() const { return _state; }
    void        reset() {
        _state = true;
        _remaining = _interval = paramBrakeBlinkPeriod.get() * 100;
        _count = paramBrakeBlinkCount.get();
    }

private:
    volatile bool       _state;
    uint8_t             _count;

    static void blink(void *arg) {
        auto b = reinterpret_cast<BrakeBlinker *>(arg);

        if (b->_state && (b->_count > 0)) {
            b->_state = false;
            b->_count--;
        } else {
            b->_state = true;
            if (b->_count == 0) {
                b->_remaining = 0;
            }
        }
    }
};

Board           board;
Master          master;
Switches        switches;
Display         disp;
Menu            menu(disp, master);

TurnBlinker     turnBlinker;
BrakeBlinker    brakeBlinker;

Decrementer     awakeDelay;
Decrementer     interiorLightsDelay;
Decrementer     pathwayLightingDelay;

bool            ignitionWasOn;

static void
interiorLights(LIN::RelayFrame &f)
{
    // door just closed - start interior lighting timer
    if (!switches.test(LIN::kSWDoor) &&
        switches.changed(LIN::kSWDoor)) {
        interiorLightsDelay.setSeconds(paramInteriorLightPeriod.get());
    }

    if (switches.test(LIN::kSWDoor) ||          // door open
        switches.test(LIN::kSWInteriorLight) || // light switch on
        (!switches.test(LIN::kSWIgnition) &&    // ignition off
         !interiorLightsDelay.expired())) {          // ... and timer not expired

        f.set(LIN::kRelayInteriorLight);
    }
}

static void
turnSignals(LIN::RelayFrame &f)
{
    // Note: if turnBlinker is disabled (period is zero) then
    // we assume that there is an external flasher in use, and
    // the switches will come and go under its control.

    // hazard warning lights?
    if (switches.test(LIN::kSWHazard)) {

        // reset turnBlinker to get a clean full flash
        if (switches.changed(LIN::kSWHazard)) {
            turnBlinker.reset();
        }
        if (turnBlinker.state()) {
            f.set(LIN::kRelayLeftTurn);
            f.set(LIN::kRelayRightTurn);
        }
        return;
    }

    // parking markers?
    if (!switches.test(LIN::kSWIgnition)) {
        // parking lights - no blink
        if (switches.test(LIN::kSWLeftTurn)) {
            f.set(LIN::kRelayLeftTurn);
        }
        if (switches.test(LIN::kSWRightTurn)) {
            f.set(LIN::kRelayRightTurn);
        }
        return;
    }

    // turn signals?
    // XXX test keep-blinking timer
    if (switches.test(LIN::kSWLeftTurn)) {
        if (switches.changed(LIN::kSWLeftTurn)) {
            turnBlinker.reset();
        }
        if (turnBlinker.state()) {
            f.set(LIN::kRelayLeftTurn);
        }
    }
    if (switches.test(LIN::kSWRightTurn)) {
        if (switches.changed(LIN::kSWRightTurn)) {
            turnBlinker.reset();
        }
        if (turnBlinker.state()) {
            f.set(LIN::kRelayRightTurn);
        }
    }
}

static void
markerLights(LIN::RelayFrame &f)
{
    // markers and city lights
    // XXX should we stay awake while these are on?
    if (switches.test(LIN::kSWMarkerLights)) {
        f.set(LIN::kRelayMarkers);
    }
}

static void
headLights(LIN::RelayFrame &f)
{
    static bool highBeamToggle;

    // lights down if nothing is on
    if (!switches.test(LIN::kSWMarkerLights) &&
        !switches.test(LIN::kSWHeadLights) &&
        !switches.test(LIN::kSWHighBeam)) {
        f.set(LIN::kRelayLightsDown);
    }

    // no lights without ignition
    // XXX marker-only mode?
    if (!switches.test(LIN::kSWIgnition)) {
        return;
    }

    // clear the highbeam toggle at ignition-on
    if (switches.changed(LIN::kSWIgnition)) {
        highBeamToggle = false;
    }

    // handle the high-beam toggle input
    if (switches.test(LIN::kSWHighBeamToggle) && 
        switches.changed(LIN::kSWHighBeamToggle)) {
        highBeamToggle = !highBeamToggle;
    }

    // test for any headlight on
    if (switches.test(LIN::kSWHeadLights) ||
        switches.test(LIN::kSWHighBeam)) {

        // lights on and popups up
        f.set(LIN::kRelayHeadLights);
        f.set(LIN::kRelayLightsUp);
        f.clear(LIN::kRelayLightsDown);

        // test for high beam; headlights off while starting
        if (!switches.test(LIN::kSWStart)) {
            if (switches.test(LIN::kSWHighBeam) ||
                highBeamToggle) {
                f.set(LIN::kRelayHighBeam);
            } else {
                f.set(LIN::kRelayLowBeam);
            }
        }
    }

    // foglights
    // XXX require other lights?
    if (switches.test(LIN::kSWFogLight)) {
        f.set(LIN::kRelayFogLights);
    }
}

static void
tailLights(LIN::RelayFrame &f)
{
    // brake lights
    // XXX test minimum-brake timer
    if (switches.test(LIN::kSWBrake)) {
        if (switches.changed(LIN::kSWBrake)) {
            brakeBlinker.reset();
        }
        if (brakeBlinker.state()) {
            f.set(LIN::kRelayBrake);
        }
    }

    // reverse lights
    if (switches.test(LIN::kSWReverse)) {
        f.set(LIN::kRelayReverse);
    }
}

static void
pathLights(LIN::RelayFrame &f)
{
    static bool ignitionWasOn;
    bool pathLightingStart = false;

    // detect ignition transition to off
    if (!switches.test(LIN::kSWIgnition) &&
        switches.changed(LIN::kSWIgnition)) {

        // door already open?
        if (switches.test(LIN::kSWDoor)) {
            pathLightingStart = true;
        } else {
            ignitionWasOn = true;
        }
    }

    // door opens after ignition off
    if (switches.test(LIN::kSWDoor) &&
        switches.changed(LIN::kSWDoor) &&
        ignitionWasOn) {
        pathLightingStart = true;
    }

    // ignition off and alarm unlock changed?
    if (!switches.test(LIN::kSWIgnition) &&
        switches.changed(LIN::kSWDoorUnlock)) {
        pathLightingStart = true;
    }

    // start path lighting?
    if (pathLightingStart) {
        pathLightingStart = false;
        ignitionWasOn = false;

        pathwayLightingDelay.setSeconds(paramPathLightPeriod.get());
    }

    // path lights on?
    if (!pathwayLightingDelay.expired()) {
        f.set(LIN::kRelayMarkers);
        // XXX other pathway lights?
    }
}

void
main(void)
{

    // check for recovery mode before doing anything else
    if (Board::getMode() != 0) {
        Board::panic(Board::kPanicRecovery);
    }

    // enable interrupts; timers and LIN events will start.
    sei();

    // check for an attached display, run setup mode if attached
    if (disp.probe()) {

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

        interiorLights(f);
        turnSignals(f);
        markerLights(f);
        headLights(f);
        tailLights(f);
        pathLights(f);

#if 0
            // doors just opened with ignition off
            if (switches.test(LIN::kSWDoor) &&
                switches.changed(LIN::kSWDoor)) {
                if (ignitionWasOn) {
                    // start path lighting timer
                    pathLightingStart = Timer::timeNow();
                }
            }

            // path lighting timer has not expired
            if (Timer::timeSince(pathLightingStart) < (paramPathLightPeriod.get() * 1000U)) {
                f.set(LIN::kRelayMarkers);
                f.set(LIN::kRelayCityLights);
            }
#endif

        // update the copy we are sending to nodes
        master.relayFrame.copy(f);
    }
}

