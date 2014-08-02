// AUTOMATICALLY GENERATED, DO NOT EDIT
#pragma once
#ifdef __AVR__
# include <avr/pgmspace.h>
#else
# define PROGMEM
#endif
#include <stdint.h>

static const uint8_t protocolRevision = 1;

enum DataPage : uint8_t {
    kDataPageInfo,
    kDataPageStatus,
    kDataPageLINErrors,
    kDataPageNodeStatus,
    kDataPageNodeParameters,
    kDataPageMax
};

static PROGMEM const char namesForDataPage[] =
    "Info\0"
    "Status\0"
    "LINErrors\0"
    "NodeStatus\0"
    "NodeParameters\0"
    "\0";

enum NodeInfo : uint8_t {
    kNodeInfoProtocolVersion,
    kNodeInfoBoardFunction,
    kNodeInfoBootloaderMode,
    kNodeInfoFirmwareVersion,
    kNodeInfoFirmwarePageSize,
    kNodeInfoMax
};

static PROGMEM const char namesForNodeInfo[] =
    "ProtocolVersion\0"
    "BoardFunction\0"
    "BootloaderMode\0"
    "FirmwareVersion\0"
    "FirmwarePageSize\0"
    "\0";

enum BasicStatus : uint8_t {
    kBasicStatusWatchdogResets,
    kBasicStatusSupplyVoltage,
    kBasicStatusTemperature,
    kBasicStatusMax
};

static PROGMEM const char namesForBasicStatus[] =
    "WatchdogResets\0"
    "SupplyVoltage\0"
    "Temperature\0"
    "\0";

enum LINError : uint8_t {
    kLINErrorLine,
    kLINErrorChecksum,
    kLINErrorParity,
    kLINErrorFraming,
    kLINErrorSynch,
    kLINErrorProtocol,
    kLINErrorMax
};

static PROGMEM const char namesForLINError[] =
    "Line\0"
    "Checksum\0"
    "Parity\0"
    "Framing\0"
    "Synch\0"
    "Protocol\0"
    "\0";

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
    kSwitchIDMax
};

static PROGMEM const char namesForSwitchID[] =
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
    "\0";

enum MasterStatus : uint8_t {
    kMasterStatusSummary,
    kMasterStatusMax
};

static PROGMEM const char namesForMasterStatus[] =
    "Summary\0"
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
    kRelayIDCabinFan4,
    kRelayIDWiperLow,
    kRelayIDWiperHigh,
    kRelayIDRearDefrost,
    kRelayIDUnassigned,
    kRelayIDMax
};

static PROGMEM const char namesForRelayID[] =
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
    "CabinFan4\0"
    "WiperLow\0"
    "WiperHigh\0"
    "RearDefrost\0"
    "Unassigned\0"
    "\0";

enum RelayFault : uint8_t {
    kRelayFaultNone,
    kRelayFaultOpenCircuit,
    kRelayFaultOverCurrent,
    kRelayFaultOverTemperature,
    kRelayFaultShortToGround,
    kRelayFaultShortToBattery,
    kRelayFaultMax
};

static PROGMEM const char namesForRelayFault[] =
    "None\0"
    "OpenCircuit\0"
    "OverCurrent\0"
    "OverTemperature\0"
    "ShortToGround\0"
    "ShortToBattery\0"
    "\0";

enum RelayStatus : uint8_t {
    kRelayStatusSummary,
    kRelayStatusRelay1Faults,
    kRelayStatusRelay2Faults,
    kRelayStatusRelay3Faults,
    kRelayStatusRelay4Faults,
    kRelayStatusRelay5Faults,
    kRelayStatusRelay6Faults,
    kRelayStatusRelay7Faults,
    kRelayStatusRelay8Faults,
    kRelayStatusRelay1PresentFault,
    kRelayStatusRelay2PresentFault,
    kRelayStatusRelay3PresentFault,
    kRelayStatusRelay4PresentFault,
    kRelayStatusRelay5PresentFault,
    kRelayStatusRelay6PresentFault,
    kRelayStatusRelay7PresentFault,
    kRelayStatusRelay8PresentFault,
    kRelayStatusRelay1LoadCurrent,
    kRelayStatusRelay2LoadCurrent,
    kRelayStatusRelay3LoadCurrent,
    kRelayStatusRelay4LoadCurrent,
    kRelayStatusRelay5LoadCurrent,
    kRelayStatusRelay6LoadCurrent,
    kRelayStatusRelay7LoadCurrent,
    kRelayStatusRelay8LoadCurrent,
    kRelayStatusMax
};

static PROGMEM const char namesForRelayStatus[] =
    "Summary\0"
    "Relay1Faults\0"
    "Relay2Faults\0"
    "Relay3Faults\0"
    "Relay4Faults\0"
    "Relay5Faults\0"
    "Relay6Faults\0"
    "Relay7Faults\0"
    "Relay8Faults\0"
    "Relay1PresentFault\0"
    "Relay2PresentFault\0"
    "Relay3PresentFault\0"
    "Relay4PresentFault\0"
    "Relay5PresentFault\0"
    "Relay6PresentFault\0"
    "Relay7PresentFault\0"
    "Relay8PresentFault\0"
    "Relay1LoadCurrent\0"
    "Relay2LoadCurrent\0"
    "Relay3LoadCurrent\0"
    "Relay4LoadCurrent\0"
    "Relay5LoadCurrent\0"
    "Relay6LoadCurrent\0"
    "Relay7LoadCurrent\0"
    "Relay8LoadCurrent\0"
    "\0";

enum RelayType : uint8_t {
    kRelayType5AGeneric,
    kRelayType10AGeneric,
    kRelayTypeLED,
    kRelayTypeHID,
    kRelayTypeLowPowerBulb,
    kRelayTypeHighPowerBulb,
    kRelayTypeMotor,
    kRelayTypeMax
};

static PROGMEM const char namesForRelayType[] =
    "5AGeneric\0"
    "10AGeneric\0"
    "LED\0"
    "HID\0"
    "LowPowerBulb\0"
    "HighPowerBulb\0"
    "Motor\0"
    "\0";


