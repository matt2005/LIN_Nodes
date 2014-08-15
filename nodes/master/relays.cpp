/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <avr/interrupt.h>

#include "board.h"
#include "bitarray.h"
#include "util.h"

#include "switches.h"
#include "master.h"

#include "lin_defs.h"

using namespace Master;

namespace Relays
{

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
        _count = Parameter(kParamPassingBlinkCount).get();
        _remaining = _interval = Parameter(kParamTurnBlinkPeriod).get() * 10U;
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
        _remaining = _interval = Parameter(kParamBrakeBlinkPeriod).get() * 10U;
        _count = Parameter(kParamBrakeBlinkCount).get();
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
            _remaining = Parameter(kParamWiperInterval).get() * 100U;
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

    void                reset() { set_milliseconds(kStayAwakeTime); }

private:
    static const Timer::Timeval kStayAwakeTime = 50000U;
};

static TurnBlinker      turnBlinker;
static BrakeBlinker     brakeBlinker;
static StayAwakeTimer   awakeDelay;
static WiperDelay       wiperDelay;
static Decrementer      interiorLightsDelay;
static Decrementer      pathwayLightingDelay;

static void
powerSignals(Response &f)
{
    // ignition on?
    if (Switches::test(input::kIgnition)) {
        Signal::Ignition(f).set();

        // start switch on?
        if (Switches::test(input::kStart)) {
            Signal::Start(f).set();
        }
    }
}

static void
markerLights(Response &f)
{
    // markers and city lights?
    if (Switches::test(input::kMarkerLights)) {

        awakeDelay.reset();                     // markers are on, stay awake

        Signal::MarkerLights(f).set();

        // Note: overridden by turnSignals below
        Signal::LeftTurnMarker(f).set();
        Signal::RightTurnMarker(f).set();
    }
}

static void
turnSignals(Response &f)
{
    // Note: if turnBlinker is disabled (period is zero) then
    // we assume that there is an external flasher in use, and
    // the switches will come and go under its control.

    // external blinker mode?
    if ((Parameter(kParamTurnBlinkPeriod).get() == 0) ||
        (Parameter(kParamTurnBlinkPeriod).get() > 100U)) {

        if (Switches::test(input::kLeftTurn)) {
            Signal::LeftTurn(f).set();
            Signal::LeftTurnMarker(f).clear();
        }

        if (Switches::test(input::kRightTurn)) {
            Signal::RightTurn(f).set();
            Signal::RightTurnMarker(f).clear();
        }

        return;
    }

    // smart hazard warning lights?
    if (Switches::test(input::kHazard)) {

        awakeDelay.reset();                     // hazards are on, stay awake

        // start the blinker
        if (Switches::changed(input::kHazard)) {
            turnBlinker.start();
        }

        // both signals track the blinker
        if (turnBlinker.state()) {
            Signal::LeftTurn(f).set();
            Signal::RightTurn(f).set();
        }

        return;

    } else {

        // hazard blinker just turned off?
        if (Switches::changed(input::kHazard)) {
            turnBlinker.cancel();
        }

        // XXX this isn't quite right; if a smart turn signal was engaged while
        //     hazards were on, we will miss it...
    }

    // parking markers?
    if (!Switches::test(input::kIgnition)) {

        // parking lights - no blink
        if (Switches::test(input::kLeftTurn)) {
            awakeDelay.reset();                     // parking markers are on, stay awake
            Signal::LeftTurn(f).set();
        }

        if (Switches::test(input::kRightTurn)) {
            awakeDelay.reset();                     // parking markers are on, stay awake
            Signal::RightTurn(f).set();
        }

        return;
    }

    // smart turn signals?
    if (Switches::test(input::kIgnition)) {
        static bool blinkLeft;

        // cancel/restart blinker
        if (Switches::changed_to_on(input::kLeftTurn)) {
            turnBlinker.start();
            blinkLeft = true;
        }

        if (Switches::changed_to_on(input::kRightTurn)) {
            turnBlinker.start();
            blinkLeft = false;
        }

        // stop blinker
        if (Switches::changed_to_off(input::kLeftTurn) ||
            Switches::changed_to_off(input::kRightTurn)) {
            turnBlinker.stop();
        }

        // signals on to suit
        if (turnBlinker.state()) {
            if (blinkLeft) {
                Signal::LeftTurn(f).set();
                Signal::RightTurn(f).clear();
            } else {
                Signal::RightTurn(f).set();
                Signal::LeftTurn(f).clear();                
            }
        }
    }
}

static void
headLights(Response &f)
{
    static bool highBeamToggle;

    // lights up if forced
    if (Switches::test(input::kLightsUp)) {
        Signal::LightsUp(f).set();

        // otherwise lights down if nothing is on

    } else if (!Switches::test(input::kMarkerLights) &&
               !Switches::test(input::kHeadLights) &&
               !Switches::test(input::kHighBeam)) {
        Signal::LightsDown(f).set();
    }

    // force lights up for test/maintenance

    // no headlights without ignition
    if (!Switches::test(input::kIgnition)) {
        return;
    }

    // clear the highbeam toggle when ignition turns on
    if (Switches::changed(input::kIgnition)) {
        highBeamToggle = false;
    }

    // handle the high-beam toggle input
    if (Switches::changed_to_on(input::kHighBeamToggle)) {
        highBeamToggle = !highBeamToggle;
    }

    // test for any headlight on
    if (Switches::test(input::kHeadLights) ||
        Switches::test(input::kHighBeam)) {

        // headlights and markers on and popups up
        Signal::HeadLights(f).set();
        Signal::MarkerLights(f).set();
        Signal::LightsUp(f).set();
        Signal::LightsDown(f).clear();         // for safety

        // test for high beam; headlights off while starting
        if (!Switches::test(input::kStart)) {
            if (Switches::test(input::kHighBeam) ||
                highBeamToggle) {
                Signal::HighBeam(f).set();

            } else {
                Signal::LowBeam(f).set();
            }
        }
    }

    // foglights
    // XXX require other lights?
    if (Switches::test(input::kFogLight)) {
        Signal::FogLights(f).set();
    }
}

static void
tailLights(Response &f)
{
    // brake lights
    if (Switches::test(input::kBrake)) {

        // start the blinker when the switch comes on
        if (Switches::changed(input::kBrake)) {
            brakeBlinker.start();
        }

        // brake light on if the blinker & pedal both agree
        if (brakeBlinker.state()) {
            Signal::Brake(f).set();
        }
    }

    // reverse lights
    if (Switches::test(input::kReverse)) {
        Signal::Reverse(f).set();
    }
}

static void
interiorLights(Response &f)
{
    // door just closed - start interior lighting timer
    if (Switches::changed_to_off(input::kDoor)) {
        interiorLightsDelay.set_seconds(Parameter(kParamInteriorLightTime).get());
    }

    // if ignition is on, cancel interior light timer
    if (Switches::test(input::kIgnition)) {
        interiorLightsDelay.clear();
    }

    // interior light on?
    if (Switches::test(input::kDoor) ||          // door open or
        Switches::test(input::kInteriorLight) || // light switch on or
        !interiorLightsDelay.expired()) {        // timer not expired

        awakeDelay.reset();                     // lights are on, stay awake

        Signal::InteriorLight(f).set();

        // XXX nice to have a time limit on the interior light when
        //     ignition is off to prevent battery drain
    }
}

static void
pathLights(Response &f)
{
    static bool ignitionWasOn;

    // detect ignition transition to off
    if (Switches::changed_to_off(input::kIgnition)) {

        // door already open?
        if (Switches::test(input::kDoor)) {
            // path lighting
            pathwayLightingDelay.set_seconds(Parameter(kParamPathLightTime).get());

        } else {
            ignitionWasOn = true;
        }
    }

    // door opens after ignition off
    if (Switches::changed_to_on(input::kDoor) &&
        ignitionWasOn) {

        // path lighting
        pathwayLightingDelay.set_seconds(Parameter(kParamPathLightTime).get());

        // XXX no path lighting after a 'false alarm' door opening
        //     might want to keep this set until sleep?
        ignitionWasOn = false;
    }

    // ignition off and alarm unlock changed?
    if (!Switches::test(input::kIgnition) &&
        Switches::changed(input::kDoorUnlock)) {

        // welcome lighting
        pathwayLightingDelay.set_seconds(Parameter(kParamWelcomeLightTime).get());
    }

    // path lights on?
    if (!pathwayLightingDelay.expired()) {

        awakeDelay.reset();                     // path lights are on, stay awake

        Signal::MarkerLights(f).set();
        // XXX other pathway lights? town lights?
    }
}

static void
climateControl(Response &f)
{
    if (Switches::test(input::kIgnition) &&
        !Switches::test(input::kStart)) {
        if (Switches::test(input::kCabinFan1)) {
            Signal::CabinFan1(f).set();
        }

        if (Switches::test(input::kCabinFan2)) {
            Signal::CabinFan2(f).set();
        }

        if (Switches::test(input::kCabinFan3)) {
            Signal::CabinFan3(f).set();
        }

        if (Switches::test(input::kRearDefrost)) {
            Signal::RearDefrost(f).set();
        }
    }
}

static void
windowWipers(Response &f)
{
    if (Switches::test(input::kIgnition) &&
        !Switches::test(input::kStart)) {

        // avoid issues with overlap between wiper switch settings
        if (Switches::changed_to_on(input::kWiperInt)) {
            wiperDelay.reset();
        }

        if (Switches::test(input::kWiperHigh)) {
            Signal::WiperHigh(f).set();

        } else if (Switches::test(input::kWiperLow)) {
            Signal::WiperLow(f).set();

        } else if (Switches::test(input::kWiperInt)) {
            if (wiperDelay.state()) {
                Signal::WiperLow(f).set();
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
    if (Switches::test(input::kIgnition) ||
        Switches::test(input::kHazard) ||
        Switches::test(input::kLeftTurn) ||
        Switches::test(input::kRightTurn) ||
        Switches::test(input::kDoor) ||
        Switches::changed(input::kDoorUnlock)) {

        awakeDelay.reset();
        gMaster.set_sleep_enable(false);
    }

    // If the awake timer has expired, allow the master
    // to sleep and do nothing more here.
    if (awakeDelay.expired()) {
        gMaster.set_sleep_enable(true);
        return;
    }

    // update the relays frame by looking at switches
    Response f;

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

    // atomically update the copy we are sending to nodes
    cli();
    gMaster.relayFrame = f;
    sei();

}

} // namespace Relays
