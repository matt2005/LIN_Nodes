
#include "board.h"

#include "switches.h"
#include "master.h"

namespace Relays
{

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

static TurnBlinker     turnBlinker;
static BrakeBlinker    brakeBlinker;


static const Timer::Timeval kStayAwakeTime = 10000;   // XXX 10 seconds is too short
static Decrementer     awakeDelay(kStayAwakeTime);
static Decrementer     interiorLightsDelay;
static Decrementer     pathwayLightingDelay;

static void
interiorLights(LIN::RelayFrame &f)
{
    // door just closed - start interior lighting timer
    if (!Switches::test(LIN::kSWDoor) &&
        Switches::changed(LIN::kSWDoor)) {
        interiorLightsDelay.setSeconds(paramInteriorLightPeriod.get());
    }

    if (Switches::test(LIN::kSWDoor) ||          // door open
        Switches::test(LIN::kSWInteriorLight) || // light switch on
        (!Switches::test(LIN::kSWIgnition) &&    // ignition off
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
    if (Switches::test(LIN::kSWHazard)) {

        // reset turnBlinker to get a clean full flash
        if (Switches::changed(LIN::kSWHazard)) {
            turnBlinker.reset();
        }
        if (turnBlinker.state()) {
            f.set(LIN::kRelayLeftTurn);
            f.set(LIN::kRelayRightTurn);
        }
        return;
    }

    // parking markers?
    if (!Switches::test(LIN::kSWIgnition)) {
        // parking lights - no blink
        if (Switches::test(LIN::kSWLeftTurn)) {
            f.set(LIN::kRelayLeftTurn);
        }
        if (Switches::test(LIN::kSWRightTurn)) {
            f.set(LIN::kRelayRightTurn);
        }
        return;
    }

    // turn signals?
    // XXX test keep-blinking timer
    if (Switches::test(LIN::kSWLeftTurn)) {
        if (Switches::changed(LIN::kSWLeftTurn)) {
            turnBlinker.reset();
        }
        if (turnBlinker.state()) {
            f.set(LIN::kRelayLeftTurn);
        }
    }
    if (Switches::test(LIN::kSWRightTurn)) {
        if (Switches::changed(LIN::kSWRightTurn)) {
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
    if (Switches::test(LIN::kSWMarkerLights)) {
        f.set(LIN::kRelayMarkers);
    }
}

static void
headLights(LIN::RelayFrame &f)
{
    static bool highBeamToggle;

    // lights down if nothing is on
    if (!Switches::test(LIN::kSWMarkerLights) &&
        !Switches::test(LIN::kSWHeadLights) &&
        !Switches::test(LIN::kSWHighBeam)) {
        f.set(LIN::kRelayLightsDown);
    }

    // no lights without ignition
    // XXX marker-only mode?
    if (!Switches::test(LIN::kSWIgnition)) {
        return;
    }

    // clear the highbeam toggle at ignition-on
    if (Switches::changed(LIN::kSWIgnition)) {
        highBeamToggle = false;
    }

    // handle the high-beam toggle input
    if (Switches::test(LIN::kSWHighBeamToggle) && 
        Switches::changed(LIN::kSWHighBeamToggle)) {
        highBeamToggle = !highBeamToggle;
    }

    // test for any headlight on
    if (Switches::test(LIN::kSWHeadLights) ||
        Switches::test(LIN::kSWHighBeam)) {

        // lights on and popups up
        f.set(LIN::kRelayHeadLights);
        f.set(LIN::kRelayLightsUp);
        f.clear(LIN::kRelayLightsDown);

        // test for high beam; headlights off while starting
        if (!Switches::test(LIN::kSWStart)) {
            if (Switches::test(LIN::kSWHighBeam) ||
                highBeamToggle) {
                f.set(LIN::kRelayHighBeam);
            } else {
                f.set(LIN::kRelayLowBeam);
            }
        }
    }

    // foglights
    // XXX require other lights?
    if (Switches::test(LIN::kSWFogLight)) {
        f.set(LIN::kRelayFogLights);
    }
}

static void
tailLights(LIN::RelayFrame &f)
{
    // brake lights
    // XXX test minimum-brake timer
    if (Switches::test(LIN::kSWBrake)) {
        if (Switches::changed(LIN::kSWBrake)) {
            brakeBlinker.reset();
        }
        if (brakeBlinker.state()) {
            f.set(LIN::kRelayBrake);
        }
    }

    // reverse lights
    if (Switches::test(LIN::kSWReverse)) {
        f.set(LIN::kRelayReverse);
    }
}

static void
pathLights(LIN::RelayFrame &f)
{
    static bool ignitionWasOn;
    bool pathLightingStart = false;

    // detect ignition transition to off
    if (!Switches::test(LIN::kSWIgnition) &&
        Switches::changed(LIN::kSWIgnition)) {

        // door already open?
        if (Switches::test(LIN::kSWDoor)) {
            pathLightingStart = true;
        } else {
            ignitionWasOn = true;
        }
    }

    // door opens after ignition off
    if (Switches::test(LIN::kSWDoor) &&
        Switches::changed(LIN::kSWDoor) &&
        ignitionWasOn) {
        pathLightingStart = true;
    }

    // ignition off and alarm unlock changed?
    if (!Switches::test(LIN::kSWIgnition) &&
        Switches::changed(LIN::kSWDoorUnlock)) {
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
tick()
{
    // Reset the awake timer if:
    // - ignition is on
    // - doors are open
    // - remote lock/unlock signal just changed
    if (Switches::test(LIN::kSWIgnition) ||
        Switches::test(LIN::kSWDoor) ||
        Switches::changed(LIN::kSWDoorUnlock)) {

        awakeDelay.setMilliseconds(kStayAwakeTime);
        gMaster.setSleep(false);

    } else {
        // If the awake timer has expired, allow the master
        // to sleep.
        if (awakeDelay.expired()) {
            gMaster.setSleep(true);
        }
    }

    // update the relays frame by looking at switches
    LIN::RelayFrame f;

    interiorLights(f);
    turnSignals(f);
    markerLights(f);
    headLights(f);
    tailLights(f);
    pathLights(f);

    // update the copy we are sending to nodes
    gMaster.relayFrame.copy(f);

}

} // namespace Relays
