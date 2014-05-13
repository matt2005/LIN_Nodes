
#include "board.h"
#include "util.h"

#include "switches.h"
#include "master.h"

namespace Relays
{

typedef Util::Bitarray<64> RelayBits;

class TurnBlinker : public Timer
{
public:
    TurnBlinker() :
        Timer(blink, this),
        _state(true)
    {}
    bool        state() const { return _state; }
    void        start() 
    {
        _state = true;
        _active = true;
        _count = paramPassingBlinkCount.get();
        _remaining = _interval = paramTurnBlinkPeriod.get() * 10U;
    }
    void        stop() {
        _active = false;
        if (_count == 0) {
            cancel();
        }
    }
    void        cancel() {
        _state = false;
        _remaining = _interval = 0;
    }

private:
    volatile bool       _state:1;
    volatile bool       _active:1;
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
        _state(true),
        _count(0)
    {}

    bool        state() const { return _state; }
    void        start() 
    {
        _state = true;
        _remaining = _interval = paramBrakeBlinkPeriod.get() * 10U;
        _count = paramBrakeBlinkCount.get();
    }

private:
    volatile bool       _state;
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
    bool        _state;

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
            _remaining = paramWiperInterval.get() * 100U;
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

// relay                    set in
// ---------------------------------------------------
// kRelayIgnition           powerSignals
// kRelayStart              powerSignals
// kRelayLightsUp           headLights
// kRelayLightsDown         headLights
// kRelayHeadLights         headLights
// kRelayLowBeam            headLights
// kRelayHighBeam           headLights
// kRelayFogLights          headLights
// kRelayMarkers            headLights markerLights
// kRelayLeftTurn           turnSignals
// kRelayLeftTurnMarker     markerLights turnSignals
// kRelayRightTurn          turnSignals
// kRelayRightTurnMarker    markerLights turnSignals
// kRelayBrake              tailLights
// kRelayReverse            tailLights
// kRelayInteriorLight      interiorLights

static void
powerSignals(RelayBits &f)
{
    // ignition on?
    if (Switches::test(LIN::kSWIgnition)) {
        f.set(LIN::kRelayIgnition);

        // start switch on?
        if (Switches::test(LIN::kSWStart)) {
            f.set(LIN::kRelayStart);            
        }
    }
}

static void
markerLights(RelayBits &f)
{
    // markers and city lights?
    if (Switches::test(LIN::kSWMarkerLights)) {

        awakeDelay.reset();                     // markers are on, stay awake

        f.set(LIN::kRelayMarkers);

        // Note: overridden by turnSignals below
        f.set(LIN::kRelayLeftTurnMarker);
        f.set(LIN::kRelayRightTurnMarker);
    }
}

static void
turnSignals(RelayBits &f)
{
    // Note: if turnBlinker is disabled (period is zero) then
    // we assume that there is an external flasher in use, and
    // the switches will come and go under its control.

    // external blinker mode?
    if ((paramTurnBlinkPeriod.get() == 0) ||
        (paramTurnBlinkPeriod.get() > 100)) {

       if (Switches::test(LIN::kSWLeftTurn)) {
           f.set(LIN::kRelayLeftTurn);
           f.clear(LIN::kRelayLeftTurnMarker);
       }
       if (Switches::test(LIN::kSWRightTurn)) {
           f.set(LIN::kRelayRightTurn);
           f.clear(LIN::kRelayRightTurnMarker);
       }
       return;
    } 

    // smart hazard warning lights?
    if (Switches::test(LIN::kSWHazard)) {

        awakeDelay.reset();                     // hazards are on, stay awake

        // start the blinker
        if (Switches::changed(LIN::kSWHazard)) {
            turnBlinker.start();
        }

        // both signals track the blinker
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
            awakeDelay.reset();                     // parking markers are on, stay awake
            f.set(LIN::kRelayLeftTurn);
        }
        if (Switches::test(LIN::kSWRightTurn)) {
            awakeDelay.reset();                     // parking markers are on, stay awake
            f.set(LIN::kRelayRightTurn);
        }
        return;
    }

    // smart turn signals?
    if (Switches::test(LIN::kSWIgnition)) {
        static bool blinkLeft;

        // cancel/restart blinker
        if (Switches::changedToOn(LIN::kSWLeftTurn)) {
            turnBlinker.start();
            blinkLeft = true;
        }
        if (Switches::changedToOn(LIN::kSWRightTurn)) {
            turnBlinker.start();
            blinkLeft = false;
        }

        // stop blinker
        if (Switches::changedToOff(LIN::kSWLeftTurn) ||
            Switches::changedToOff(LIN::kSWRightTurn)) {
            turnBlinker.stop();
        }

        // signals on to suit
        if (turnBlinker.state()) {
           f.set(blinkLeft ? LIN::kRelayLeftTurn : LIN::kRelayRightTurn);
           f.clear(blinkLeft ? LIN::kRelayLeftTurnMarker : LIN::kRelayRightTurnMarker);
        }
    }
}

static void
headLights(RelayBits &f)
{
    static bool highBeamToggle;

    // lights up if forced
    if (Switches::test(LIN::kSWLightsUp)) {
        f.set(LIN::kRelayLightsUp);

    // otherwise lights down if nothing is on
    } else if (!Switches::test(LIN::kSWMarkerLights) &&
        !Switches::test(LIN::kSWHeadLights) &&
        !Switches::test(LIN::kSWHighBeam)) {
        f.set(LIN::kRelayLightsDown);
    }

    // force lights up for test/maintenance

    // no headlights without ignition
    if (!Switches::test(LIN::kSWIgnition)) {
        return;
    }

    // clear the highbeam toggle when ignition turns on
    if (Switches::changed(LIN::kSWIgnition)) {
        highBeamToggle = false;
    }

    // handle the high-beam toggle input
    if (Switches::changedToOn(LIN::kSWHighBeamToggle)) {
        highBeamToggle = !highBeamToggle;
    }

    // test for any headlight on
    if (Switches::test(LIN::kSWHeadLights) ||
        Switches::test(LIN::kSWHighBeam)) {

        // headlights and markers on and popups up
        f.set(LIN::kRelayHeadLights);
        f.set(LIN::kRelayMarkers);
        f.set(LIN::kRelayLightsUp);
        f.clear(LIN::kRelayLightsDown);         // for safety

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
tailLights(RelayBits &f)
{
    // brake lights
    if (Switches::test(LIN::kSWBrake)) {

        // start the blinker when the switch comes on
        if (Switches::changed(LIN::kSWBrake)) {
            brakeBlinker.start();
        }

        // brake light on if the blinker & pedal both agree
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
interiorLights(RelayBits &f)
{
    // door just closed - start interior lighting timer
    if (Switches::changedToOff(LIN::kSWDoor)) {
        interiorLightsDelay.setSeconds(paramInteriorLightPeriod.get());
    }

    // if ignition is on, cancel interior light timer
    if (Switches::test(LIN::kSWIgnition)) {
        interiorLightsDelay.clear();
    }

    // interior light on?
    if (Switches::test(LIN::kSWDoor) ||          // door open or
        Switches::test(LIN::kSWInteriorLight) || // light switch on or
        !interiorLightsDelay.expired()) {        // timer not expired

        awakeDelay.reset();                     // lights are on, stay awake

        f.set(LIN::kRelayInteriorLight);
    }
}

static void
pathLights(RelayBits &f)
{
    static bool ignitionWasOn;

    // detect ignition transition to off
    if (Switches::changedToOff(LIN::kSWIgnition)) {

        // door already open?
        if (Switches::test(LIN::kSWDoor)) {
            // path lighting
            pathwayLightingDelay.setSeconds(paramPathLightPeriod.get());
        } else {
            ignitionWasOn = true;
        }
    }

    // door opens after ignition off
    if (Switches::changedToOn(LIN::kSWDoor) &&
        ignitionWasOn) {

        // path lighting
        pathwayLightingDelay.setSeconds(paramPathLightPeriod.get());

        // XXX no path lighting after a 'false alarm' door opening
        //     might want to keep this set until sleep?
        ignitionWasOn = false;
    }

    // ignition off and alarm unlock changed?
    if (!Switches::test(LIN::kSWIgnition) &&
        Switches::changed(LIN::kSWDoorUnlock)) {

        // welcome lighting
        pathwayLightingDelay.setSeconds(paramWelcomeLightPeriod.get());
    }

    // path lights on?
    if (!pathwayLightingDelay.expired()) {

        awakeDelay.reset();                     // path lights are on, stay awake

        f.set(LIN::kRelayMarkers);
        // XXX other pathway lights? town lights?
    }
}

static void
climateControl(RelayBits f)
{
    if (Switches::test(LIN::kSWIgnition) &&
        !Switches::test(LIN::kSWStart)) {
        if (Switches::test(LIN::kSWCabinFan1)) {
            f.set(LIN::kRelayCabinFan1);
        }
        if (Switches::test(LIN::kSWCabinFan2)) {
            f.set(LIN::kRelayCabinFan2);
        }
        if (Switches::test(LIN::kSWCabinFan3)) {
            f.set(LIN::kRelayCabinFan3);
        }
        if (Switches::test(LIN::kSWRearDefrost)) {
            f.set(LIN::kRelayRearDefrost);
        }
    }
}

static void
windowWipers(RelayBits f)
{
    if (Switches::test(LIN::kSWIgnition) &&
        !Switches::test(LIN::kSWStart)) {

        // avoid issues with overlap between wiper switch settings
        if (Switches::changedToOn(LIN::kSWWiperInt)) {
            wiperDelay.reset();
        }

        if (Switches::test(LIN::kSWWiperHigh)) {
            f.set(LIN::kRelayWiperHigh);
        } else if (Switches::test(LIN::kSWWiperLow)) {
            f.set(LIN::kRelayWiperLow);
        } else if (Switches::test(LIN::kSWWiperInt)) {
            if (wiperDelay.state()) {
                f.set(LIN::kRelayWiperLow);                
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
    if (Switches::test(LIN::kSWIgnition) ||
        Switches::test(LIN::kSWHazard) ||
        Switches::test(LIN::kSWLeftTurn) ||
        Switches::test(LIN::kSWRightTurn) ||
        Switches::test(LIN::kSWDoor) ||
        Switches::changed(LIN::kSWDoorUnlock)) {

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
