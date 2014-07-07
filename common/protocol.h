// AUTOMATICALLY GENERATED, DO NOT EDIT
#pragma once
#ifdef __AVR__
# include <avr/pgmspace.h>
#else
# define PROGMEM
#endif
#include <stdint.h>

namespace LIN {
static const uint8_t protocolRevision = 1;

enum FrameID : uint8_t {
    kFIDNone = 0x00,
    kFIDRelays = 0x01,
    kFIDConfigRequest = 0x2c,
    kFIDConfigResponse = 0x2d,
    kFIDMasterRequest = 0x3c,
    kFIDSlaveResponse = 0x3d,
    kFIDTest = 0x3f,
};

enum FrameLen : uint8_t {
    kFLenNone = 0,
    kFLenRelays = 8,
    kFLenConfigRequest = 8,
    kFLenConfigResponse = 8,
    kFLenMasterRequest = 8,
    kFLenSlaveResponse = 8,
    kFLenTest = 8,
};

enum SwitchID : uint8_t {
    kSwitchIDIgnition,
    kSwitchIDStart,
    kSwitchIDMarkerLights,
    kSwitchIDHeadLights,
    kSwitchIDHighBeam,
    kSwitchIDHighBeamToggle,
    kSwitchIDFogLight,
    kSwitchIDLeftTurn,
    kSwitchIDRightTurn,
    kSwitchIDBrake,
    kSwitchIDReverse,
    kSwitchIDDoor,
    kSwitchIDInteriorLight,
    kSwitchIDHazard,
    kSwitchIDDoorUnlock,
    kSwitchIDLightsUp,
    kSwitchIDCabinFan1,
    kSwitchIDCabinFan2,
    kSwitchIDCabinFan3,
    kSwitchIDWiperInt,
    kSwitchIDWiperLow,
    kSwitchIDWiperHigh,
    kSwitchIDRearDefrost,
    kSwitchIDUnassigned,
    kSwitchIDMax,
};

static PROGMEM const char * const strtabSwitchID = 
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
    "Unassigned\0"
    "Max\0"
    "\0";

enum RelayID : uint8_t {
    kRelayIDIgnition,
    kRelayIDStart,
    kRelayIDLightsUp,
    kRelayIDLightsDown,
    kRelayIDHeadLights,
    kRelayIDLowBeam,
    kRelayIDHighBeam,
    kRelayIDFogLights,
    kRelayIDMarkers,
    kRelayIDLeftTurn,
    kRelayIDLeftTurnMarker,
    kRelayIDRightTurn,
    kRelayIDRightTurnMarker,
    kRelayIDBrake,
    kRelayIDReverse,
    kRelayIDInteriorLight,
    kRelayIDCabinFan1,
    kRelayIDCabinFan2,
    kRelayIDCabinFan3,
    kRelayIDWiperLow,
    kRelayIDWiperHigh,
    kRelayIDRearDefrost,
    kRelayIDUnassigned,
    kRelayIDMax,
};

static PROGMEM const char * const strtabRelayID = 
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
    "Unassigned\0"
    "Max\0"
    "\0";

enum RelayType : uint8_t {
    kRelayType5AGeneric,
    kRelayType10AGeneric,
    kRelayTypeLED,
    kRelayTypeHID,
    kRelayTypeLowPowerBulb,
    kRelayTypeHighPowerBulb,
    kRelayTypeMotor,
    kRelayTypeMax,
};

static PROGMEM const char * const strtabRelayType = 
    "5AGeneric\0"
    "10AGeneric\0"
    "LED\0"
    "HID\0"
    "LowPowerBulb\0"
    "HighPowerBulb\0"
    "Motor\0"
    "Max\0"
    "\0";


} // namespace LIN
