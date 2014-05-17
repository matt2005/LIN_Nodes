// AUTOMATICALLY GENERATED - DO NOT EDIT
#include <avr/pgmspace.h>

namespace LIN {

enum SwitchID : uint8_t {
    kSWIgnition = 0,
    kSWStart = 1,
    kSWMarkerLights = 2,
    kSWHeadLights = 3,
    kSWHighBeam = 4,
    kSWHighBeamToggle = 5,
    kSWFogLight = 6,
    kSWLeftTurn = 7,
    kSWRightTurn = 8,
    kSWBrake = 9,
    kSWReverse = 10,
    kSWDoor = 11,
    kSWInteriorLight = 12,
    kSWHazard = 13,
    kSWDoorUnlock = 14,
    kSWLightsUp = 15,
    kSWCabinFan1 = 16,
    kSWCabinFan2 = 17,
    kSWCabinFan3 = 18,
    kSWWiperInt = 19,
    kSWWiperLow = 20,
    kSWWiperHigh = 21,
    kSWRearDefrost = 22,
    kSWMax,
    kSWNone = 0xff,
};

enum RelayID : uint8_t {
    kRelayIgnition = 0,
    kRelayStart = 1,
    kRelayLightsUp = 2,
    kRelayLightsDown = 3,
    kRelayHeadLights = 4,
    kRelayLowBeam = 5,
    kRelayHighBeam = 6,
    kRelayFogLights = 7,
    kRelayMarkers = 8,
    kRelayLeftTurn = 9,
    kRelayLeftTurnMarker = 10,
    kRelayRightTurn = 11,
    kRelayRightTurnMarker = 12,
    kRelayBrake = 13,
    kRelayReverse = 14,
    kRelayInteriorLight = 15,
    kRelayCabinFan1 = 16,
    kRelayCabinFan2 = 17,
    kRelayCabinFan3 = 18,
    kRelayWiperLow = 19,
    kRelayWiperHigh = 20,
    kRelayRearDefrost = 21,
    kRelayMax,
    kRelayNone = 0xff,
};

} //namespace LIN

static PROGMEM const char switchNames[] = 
    "Ignition\0"
    "Start\0"
    "MarkerLights\0"
    "HeadLights\0"
    "HighBeam\0"
    "HighBeamToggle\0"
    "FogLight\0"
    "LeftTurn\0"
    "RightTurn\0"
    "Brake\0"
    "Reverse\0"
    "Door\0"
    "InteriorLight\0"
    "Hazard\0"
    "DoorUnlock\0"
    "LightsUp\0"
    "CabinFan1\0"
    "CabinFan2\0"
    "CabinFan3\0"
    "WiperInt\0"
    "WiperLow\0"
    "WiperHigh\0"
    "RearDefrost\0"
    "\0";

static PROGMEM const char relayNames[] = 
    "Ignition\0"
    "Start\0"
    "LightsUp\0"
    "LightsDown\0"
    "HeadLights\0"
    "LowBeam\0"
    "HighBeam\0"
    "FogLights\0"
    "Markers\0"
    "LeftTurn\0"
    "LeftTurnMarker\0"
    "RightTurn\0"
    "RightTurnMarker\0"
    "Brake\0"
    "Reverse\0"
    "InteriorLight\0"
    "CabinFan1\0"
    "CabinFan2\0"
    "CabinFan3\0"
    "WiperLow\0"
    "WiperHigh\0"
    "RearDefrost\0"
    "\0";

