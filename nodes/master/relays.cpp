
#include "board.h"
#include "util.h"

#include "switches.h"
#include "master.h"

#include "protocol.h"
#include "param_Master.h"

namespace Relays
{

typedef Util::Bitarray<64> RelayBits;

class TurnBlinker : public Timer
{
public:
    TurnBlinker() :
        Timer(blink, this),
        _state(false),
        _active(false),
        _count(0)
    {}
    bool        state() const { return _state; }
    void        start()
    {
        _state = true;
        _active = true;
        _count = paramPassingBlinkCount;
        _remaining = _interval = paramTurnBlinkPeriod * 10U;
    }
    void        stop()
    {
        _active = false;

        if (_count == 0) {
            cancel();
        }
    }
    void        cancel()
    {
        _state = false;
        _remaining = _interval = 0;
    }

private:
    volatile bool       _state: 1;
    volatile bool       _active: 1;
    volatile uint8_t    _count;

    static void blink(void *arg)
    {
        auto b = reinterpret_cast<TurnBlinker *>(arg);
        b->_blink();
    }

    void        _blink()
    {
        if (_state) {
            _state = false;

            if (_count > 0) {
                _count--;

                if (!_active && (_count == 0)) {
                    cancel();
                }
            }

        } else {
            _state = true;
        }
    }
};

class BrakeBlinker : public Timer
{
public:
    BrakeBlinker() :
        Timer(blink, this),
        _state(false),
        _count(0)
    {}

    bool        state() const { return _state; }
    void        start()
    {
        _state = true;
        _remaining = _interval = paramBrakeBlinkPeriod * 10U;
        _count = paramBrakeBlinkCount;
    }

private:
    volatile bool       _state: 1;
    uint8_t             _count;

    static void blink(void *arg)
    {
        auto b = reinterpret_cast<BrakeBlinker *>(arg);
        b->_blink();
    }
    void        _blink()
    {
        if (_state && (_count > 0)) {
            _state = false;
            _count--;

        } else {
            _state = true;

            if (_count == 0) {
                _remaining = 0;
            }
        }
    }
};

class WiperDelay : public Timer
{
public:
    WiperDelay() :
        Timer(swap, this),
        _state(false)
    {}

    void        reset() { _state = true; _swap(); }
    bool        state() const { return _state; }

private:
    bool        _state: 1;

    static void swap(void *arg)
    {
        auto t = reinterpret_cast<WiperDelay *>(arg);
        t->_swap();
    }
    void        _swap()
    {
        if (_state) {
            // wiper control signal off for the specified interval
            // XXX as feature, this should be based on an analog input...
            _remaining = paramWiperInterval * 100U;
            _state = false;

        } else {
            // wiper control signal on for 1/2 second, motor responsible
            // for completing wipe & parking
            _remaining = 500U;
            _state = true;
        }
    }
};

class StayAwakeTimer : public Decrementer
{
public:
    StayAwakeTimer() : Decrementer(kStayAwakeTime) {}

    void                reset() { setMilliseconds(kStayAwakeTime); }

private:
    static const Timer::Timeval kStayAwakeTime = 10000;   // XXX is this too short?
};

static TurnBlinker      turnBlinker;
static BrakeBlinker     brakeBlinker;
static StayAwakeTimer   awakeDelay;
static WiperDelay       wiperDelay;
static Decrementer      interiorLightsDelay;
static Decrementer      pathwayLightingDelay;

static void
powerSignals(RelayBits &f)
{
    // ignition on?
    if (Switches::test(LIN::kSwitchIDIgnition)) {
        f.set(LIN::kRelayIDIgnition);

        // start switch on?
        if (Switches::test(LIN::kSwitchIDStart)) {
            f.set(LIN::kRelayIDStart);
        }
    }
}

static void
markerLights(RelayBits &f)
{
    // markers and city lights?
    if (Switches::test(LIN::kSwitchIDMarkerLights)) {

        awakeDelay.reset();                     // markers are on, stay awake

        f.set(LIN::kRelayIDMarkers);

        // Note: overridden by turnSignals below
        f.set(LIN::kRelayIDLeftTurnMarker);
        f.set(LIN::kRelayIDRightTurnMarker);
    }
}

static void
turnSignals(RelayBits &f)
{
    // Note: if turnBlinker is disabled (period is zero) then
    // we assume that there is an external flasher in use, and
    // the switches will come and go under its control.

    // external blinker mode?
    if ((paramTurnBlinkPeriod == 0) ||
        (paramTurnBlinkPeriod > 100)) {

        if (Switches::test(LIN::kSwitchIDLeftTurn)) {
            f.set(LIN::kRelayIDLeftTurn);
            f.clear(LIN::kRelayIDLeftTurnMarker);
        }

        if (Switches::test(LIN::kSwitchIDRightTurn)) {
            f.set(LIN::kRelayIDRightTurn);
            f.clear(LIN::kRelayIDRightTurnMarker);
        }

        return;
    }

    // smart hazard warning lights?
    if (Switches::test(LIN::kSwitchIDHazard)) {

        awakeDelay.reset();                     // hazards are on, stay awake

        // start the blinker
        if (Switches::changed(LIN::kSwitchIDHazard)) {
            turnBlinker.start();
        }

        // both signals track the blinker
        if (turnBlinker.state()) {
            f.set(LIN::kRelayIDLeftTurn);
            f.set(LIN::kRelayIDRightTurn);
        }

        return;

    } else {

        // hazard blinker just turned off?
        if (Switches::changed(LIN::kSwitchIDHazard)) {
            turnBlinker.cancel();
        }

        // XXX this isn't quite right; if a smart turn signal was engaged while
        //     hazards were on, we will miss it...
    }

    // parking markers?
    if (!Switches::test(LIN::kSwitchIDIgnition)) {

        // parking lights - no blink
        if (Switches::test(LIN::kSwitchIDLeftTurn)) {
            awakeDelay.reset();                     // parking markers are on, stay awake
            f.set(LIN::kRelayIDLeftTurn);
        }

        if (Switches::test(LIN::kSwitchIDRightTurn)) {
            awakeDelay.reset();                     // parking markers are on, stay awake
            f.set(LIN::kRelayIDRightTurn);
        }

        return;
    }

    // smart turn signals?
    if (Switches::test(LIN::kSwitchIDIgnition)) {
        static bool blinkLeft;

        // cancel/restart blinker
        if (Switches::changedToOn(LIN::kSwitchIDLeftTurn)) {
            turnBlinker.start();
            blinkLeft = true;
        }

        if (Switches::changedToOn(LIN::kSwitchIDRightTurn)) {
            turnBlinker.start();
            blinkLeft = false;
        }

        // stop blinker
        if (Switches::changedToOff(LIN::kSwitchIDLeftTurn) ||
            Switches::changedToOff(LIN::kSwitchIDRightTurn)) {
            turnBlinker.stop();
        }

        // signals on to suit
        if (turnBlinker.state()) {
            f.set(blinkLeft ? LIN::kRelayIDLeftTurn : LIN::kRelayIDRightTurn);
            f.clear(blinkLeft ? LIN::kRelayIDLeftTurnMarker : LIN::kRelayIDRightTurnMarker);
        }
    }
}

static void
headLights(RelayBits &f)
{
    static bool highBeamToggle;

    // lights up if forced
    if (Switches::test(LIN::kSwitchIDLightsUp)) {
        f.set(LIN::kRelayIDLightsUp);

        // otherwise lights down if nothing is on

    } else if (!Switches::test(LIN::kSwitchIDMarkerLights) &&
               !Switches::test(LIN::kSwitchIDHeadLights) &&
               !Switches::test(LIN::kSwitchIDHighBeam)) {
        f.set(LIN::kRelayIDLightsDown);
    }

    // force lights up for test/maintenance

    // no headlights without ignition
    if (!Switches::test(LIN::kSwitchIDIgnition)) {
        return;
    }

    // clear the highbeam toggle when ignition turns on
    if (Switches::changed(LIN::kSwitchIDIgnition)) {
        highBeamToggle = false;
    }

    // handle the high-beam toggle input
    if (Switches::changedToOn(LIN::kSwitchIDHighBeamToggle)) {
        highBeamToggle = !highBeamToggle;
    }

    // test for any headlight on
    if (Switches::test(LIN::kSwitchIDHeadLights) ||
        Switches::test(LIN::kSwitchIDHighBeam)) {

        // headlights and markers on and popups up
        f.set(LIN::kRelayIDHeadLights);
        f.set(LIN::kRelayIDMarkers);
        f.set(LIN::kRelayIDLightsUp);
        f.clear(LIN::kRelayIDLightsDown);         // for safety

        // test for high beam; headlights off while starting
        if (!Switches::test(LIN::kSwitchIDStart)) {
            if (Switches::test(LIN::kSwitchIDHighBeam) ||
                highBeamToggle) {
                f.set(LIN::kRelayIDHighBeam);

            } else {
                f.set(LIN::kRelayIDLowBeam);
            }
        }
    }

    // foglights
    // XXX require other lights?
    if (Switches::test(LIN::kSwitchIDFogLight)) {
        f.set(LIN::kRelayIDFogLights);
    }
}

static void
tailLights(RelayBits &f)
{
    // brake lights
    if (Switches::test(LIN::kSwitchIDBrake)) {

        // start the blinker when the switch comes on
        if (Switches::changed(LIN::kSwitchIDBrake)) {
            brakeBlinker.start();
        }

        // brake light on if the blinker & pedal both agree
        if (brakeBlinker.state()) {
            f.set(LIN::kRelayIDBrake);
        }
    }

    // reverse lights
    if (Switches::test(LIN::kSwitchIDReverse)) {
        f.set(LIN::kRelayIDReverse);
    }
}

static void
interiorLights(RelayBits &f)
{
    // door just closed - start interior lighting timer
    if (Switches::changedToOff(LIN::kSwitchIDDoor)) {
        interiorLightsDelay.setSeconds(paramInteriorLightPeriod);
    }

    // if ignition is on, cancel interior light timer
    if (Switches::test(LIN::kSwitchIDIgnition)) {
        interiorLightsDelay.clear();
    }

    // interior light on?
    if (Switches::test(LIN::kSwitchIDDoor) ||          // door open or
        Switches::test(LIN::kSwitchIDInteriorLight) || // light switch on or
        !interiorLightsDelay.expired()) {        // timer not expired

        awakeDelay.reset();                     // lights are on, stay awake

        f.set(LIN::kRelayIDInteriorLight);

        // XXX nice to have a time limit on the interior light when
        //     ignition is off to prevent battery drain
    }
}

static void
pathLights(RelayBits &f)
{
    static bool ignitionWasOn;

    // detect ignition transition to off
    if (Switches::changedToOff(LIN::kSwitchIDIgnition)) {

        // door already open?
        if (Switches::test(LIN::kSwitchIDDoor)) {
            // path lighting
            pathwayLightingDelay.setSeconds(paramPathLightPeriod);

        } else {
            ignitionWasOn = true;
        }
    }

    // door opens after ignition off
    if (Switches::changedToOn(LIN::kSwitchIDDoor) &&
        ignitionWasOn) {

        // path lighting
        pathwayLightingDelay.setSeconds(paramPathLightPeriod);

        // XXX no path lighting after a 'false alarm' door opening
        //     might want to keep this set until sleep?
        ignitionWasOn = false;
    }

    // ignition off and alarm unlock changed?
    if (!Switches::test(LIN::kSwitchIDIgnition) &&
        Switches::changed(LIN::kSwitchIDDoorUnlock)) {

        // welcome lighting
        pathwayLightingDelay.setSeconds(paramWelcomeLightPeriod);
    }

    // path lights on?
    if (!pathwayLightingDelay.expired()) {

        awakeDelay.reset();                     // path lights are on, stay awake

        f.set(LIN::kRelayIDMarkers);
        // XXX other pathway lights? town lights?
    }
}

static void
climateControl(RelayBits f)
{
    if (Switches::test(LIN::kSwitchIDIgnition) &&
        !Switches::test(LIN::kSwitchIDStart)) {
        if (Switches::test(LIN::kSwitchIDCabinFan1)) {
            f.set(LIN::kRelayIDCabinFan1);
        }

        if (Switches::test(LIN::kSwitchIDCabinFan2)) {
            f.set(LIN::kRelayIDCabinFan2);
        }

        if (Switches::test(LIN::kSwitchIDCabinFan3)) {
            f.set(LIN::kRelayIDCabinFan3);
        }

        if (Switches::test(LIN::kSwitchIDRearDefrost)) {
            f.set(LIN::kRelayIDRearDefrost);
        }
    }
}

static void
windowWipers(RelayBits f)
{
    if (Switches::test(LIN::kSwitchIDIgnition) &&
        !Switches::test(LIN::kSwitchIDStart)) {

        // avoid issues with overlap between wiper switch settings
        if (Switches::changedToOn(LIN::kSwitchIDWiperInt)) {
            wiperDelay.reset();
        }

        if (Switches::test(LIN::kSwitchIDWiperHigh)) {
            f.set(LIN::kRelayIDWiperHigh);

        } else if (Switches::test(LIN::kSwitchIDWiperLow)) {
            f.set(LIN::kRelayIDWiperLow);

        } else if (Switches::test(LIN::kSwitchIDWiperInt)) {
            if (wiperDelay.state()) {
                f.set(LIN::kRelayIDWiperLow);
            }
        }
    }
}

void
tick()
{
    // Wake up and reset the awake timer if:
    // - ignition is on
    // - hazard warning signals are on
    // - left or right parking markers are on
    // - doors are open
    // - remote lock/unlock signal changed
    //
    if (Switches::test(LIN::kSwitchIDIgnition) ||
        Switches::test(LIN::kSwitchIDHazard) ||
        Switches::test(LIN::kSwitchIDLeftTurn) ||
        Switches::test(LIN::kSwitchIDRightTurn) ||
        Switches::test(LIN::kSwitchIDDoor) ||
        Switches::changed(LIN::kSwitchIDDoorUnlock)) {

        awakeDelay.reset();
        gMaster.setSleep(false);
    }

    // If the awake timer has expired, allow the master
    // to sleep and do nothing more here.
    if (awakeDelay.expired()) {
        gMaster.setSleep(true);
        return;
    }

    // update the relays frame by looking at switches
    RelayBits f;
    f.reset();

    // order here is important
    powerSignals(f);
    markerLights(f);
    turnSignals(f);
    headLights(f);
    tailLights(f);
    interiorLights(f);
    pathLights(f);
    climateControl(f);
    windowWipers(f);

    // update the copy we are sending to nodes
    gMaster.relayFrame.copy(f);

}

} // namespace Relays
