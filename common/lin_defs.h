// AUTOMATICALLY GENERATED - DO NOT EDIT
#pragma once
#ifdef __AVR__
# include <avr/pgmspace.h>
#else
# define PROGMEM
#endif
#include "lin_types.h"


static const uint8_t kEncoding_input = 0;
static const uint8_t kEncoding_msec = 1;
static const uint8_t kEncoding_sec = 2;
static const uint8_t kEncoding_up_to_10 = 3;
static const uint8_t kEncoding_node_address = 4;
static const uint8_t kEncoding_pci = 5;
static const uint8_t kEncoding_service_id = 6;
static const uint8_t kEncoding_service_error = 7;
static const uint8_t kEncoding_v1_output_status = 8;
static const uint8_t kEncoding_v1_output_assignment = 9;
static const uint8_t kEncoding_v3_output_status = 10;
static const uint8_t kEncoding_output_type = 11;
static const uint8_t kEncoding_output_assignment = 12;
static const uint8_t kEncoding_pwm_duty_cycle = 13;
static const uint8_t kEncoding_RPM = 14;
static const uint8_t kEncoding_PSI = 15;
static const uint8_t kEncoding_F = 16;
static const uint8_t kEncoding_V = 17;
static const uint8_t kEncoding_lambda = 18;
static const uint8_t kEncoding_MPH = 19;
static const uint8_t kEncoding_none = 0xff;

namespace input
{
static const uint16_t kUnassigned = 0;
static const uint16_t kIgnition = 1;
static const uint16_t kStart = 2;
static const uint16_t kMarkerLights = 3;
static const uint16_t kHeadLights = 4;
static const uint16_t kHighBeam = 5;
static const uint16_t kHighBeamToggle = 6;
static const uint16_t kFogLight = 7;
static const uint16_t kLeftTurn = 8;
static const uint16_t kRightTurn = 9;
static const uint16_t kBrake = 10;
static const uint16_t kReverse = 11;
static const uint16_t kDoor = 12;
static const uint16_t kInteriorLight = 13;
static const uint16_t kHazard = 14;
static const uint16_t kDoorUnlock = 15;
static const uint16_t kLightsUp = 16;
static const uint16_t kCabinFan1 = 17;
static const uint16_t kCabinFan2 = 18;
static const uint16_t kCabinFan3 = 19;
static const uint16_t kWiperInt = 20;
static const uint16_t kWiperLow = 21;
static const uint16_t kWiperHigh = 22;
static const uint16_t kRearDefrost = 23;
static const uint16_t kNumEncodings = 24;
} // namespace input

namespace msec
{
static const uint16_t kDisabled = 0;
static const uint16_t kNumEncodings = 2;
} // namespace msec

namespace sec
{
static const uint16_t kDisabled = 0;
static const uint16_t kNumEncodings = 2;
} // namespace sec

namespace up_to_10
{
static const uint16_t kDisabled = 0;
static const uint16_t kNumEncodings = 2;
} // namespace up_to_10

namespace node_address
{
static const uint16_t kSleep = 0;
static const uint16_t kFunctional = 126;
static const uint16_t kBroadcast = 127;
static const uint16_t kNumEncodings = 4;
} // namespace node_address

namespace pci
{
static const uint16_t kSingleFrame = 0;
static const uint16_t kFirstFrame = 1;
static const uint16_t kContiuationFrame = 2;
static const uint16_t kNumEncodings = 3;
} // namespace pci

namespace service_id
{
static const uint16_t kReadDataByID = 0x22;
static const uint16_t kWriteDataByID = 0x2e;
static const uint16_t kTesterPresent = 0x3e;
static const uint16_t kReadByID = 0xb2;
static const uint16_t kDataDump = 0xb4;
static const uint16_t kErrorResponse = 0x7f;
static const uint16_t kResponseOffset = 0x40;
static const uint16_t kNumEncodings = 7;
} // namespace service_id

namespace service_error
{
static const uint16_t kFunctionNotSupported = 0x12;
static const uint16_t kIncorrectLength = 0x13;
static const uint16_t kConditionsNotCorrect = 0x22;
static const uint16_t kOutOfRange = 0x31;
static const uint16_t kAccessDenied = 0x33;
static const uint16_t kGeneralFailure = 0x72;
static const uint16_t kNumEncodings = 6;
} // namespace service_error

namespace v1_output_status
{
static const uint16_t kOK = 0;
static const uint16_t kFault = 1;
static const uint16_t kNumEncodings = 2;
} // namespace v1_output_status

namespace v1_output_assignment
{
static const uint16_t kUnassigned = 0;
static const uint16_t kIgnition = 1;
static const uint16_t kStart = 2;
static const uint16_t kLightsUp = 3;
static const uint16_t kLightsDown = 4;
static const uint16_t kHeadLights = 5;
static const uint16_t kLowBeam = 6;
static const uint16_t kHighBeam = 7;
static const uint16_t kFogLights = 8;
static const uint16_t kMarkerLights = 9;
static const uint16_t kLeftTurn = 10;
static const uint16_t kLeftTurnMarker = 11;
static const uint16_t kRightTurn = 12;
static const uint16_t kRightTurnMarker = 13;
static const uint16_t kBrake = 14;
static const uint16_t kReverse = 15;
static const uint16_t kInteriorLight = 16;
static const uint16_t kCabinFan1 = 17;
static const uint16_t kCabinFan2 = 18;
static const uint16_t kCabinFan3 = 19;
static const uint16_t kCabinFan4 = 20;
static const uint16_t kWiperLow = 21;
static const uint16_t kWiperHigh = 22;
static const uint16_t kRearDefrost = 23;
static const uint16_t kNumEncodings = 24;
} // namespace v1_output_assignment

namespace v3_output_status
{
static const uint16_t kOK = 0;
static const uint16_t kOpenCircuit = 1;
static const uint16_t kOverCurrent = 2;
static const uint16_t kOverTemperature = 3;
static const uint16_t kShortToGround = 4;
static const uint16_t kShortToBattery = 5;
static const uint16_t kNumEncodings = 6;
} // namespace v3_output_status

namespace output_type
{
static const uint16_t k5AGeneric = 0;
static const uint16_t k10AGeneric = 1;
static const uint16_t kLED = 2;
static const uint16_t kHID = 3;
static const uint16_t kLowPowerBulb = 4;
static const uint16_t kHighPowerBulb = 5;
static const uint16_t kMotor = 6;
static const uint16_t kNumEncodings = 7;
} // namespace output_type

namespace output_assignment
{
static const uint16_t kUnassigned = 0;
static const uint16_t kIgnition = 1;
static const uint16_t kStart = 2;
static const uint16_t kLightsUp = 3;
static const uint16_t kLightsDown = 4;
static const uint16_t kHeadLights = 5;
static const uint16_t kLowBeam = 6;
static const uint16_t kHighBeam = 7;
static const uint16_t kFogLights = 8;
static const uint16_t kMarkerLights = 9;
static const uint16_t kLeftTurn = 10;
static const uint16_t kLeftTurnMarker = 11;
static const uint16_t kRightTurn = 12;
static const uint16_t kRightTurnMarker = 13;
static const uint16_t kBrake = 14;
static const uint16_t kReverse = 15;
static const uint16_t kInteriorLight = 16;
static const uint16_t kCabinFan1 = 17;
static const uint16_t kCabinFan2 = 18;
static const uint16_t kCabinFan3 = 19;
static const uint16_t kCabinFan4 = 20;
static const uint16_t kWiperLow = 21;
static const uint16_t kWiperHigh = 22;
static const uint16_t kRearDefrost = 23;
static const uint16_t kNumEncodings = 24;
} // namespace output_assignment

namespace pwm_duty_cycle
{
static const uint16_t kNumEncodings = 1;
} // namespace pwm_duty_cycle

namespace RPM
{
static const uint16_t kNumEncodings = 1;
} // namespace RPM

namespace PSI
{
static const uint16_t kNumEncodings = 1;
} // namespace PSI

namespace F
{
static const uint16_t kNumEncodings = 1;
} // namespace F

namespace V
{
static const uint16_t kNumEncodings = 1;
} // namespace V

namespace lambda
{
static const uint16_t kNumEncodings = 1;
} // namespace lambda

namespace MPH
{
static const uint16_t kNumEncodings = 1;
} // namespace MPH

namespace Signal
{
class nad : public SignalBase
{
public:
    constexpr nad(Response &r) : SignalBase(r, 0, 8, kEncoding_node_address) {}
};
class length : public SignalBase
{
public:
    constexpr length(Response &r) : SignalBase(r, 8, 4, kEncoding_none) {}
};
class pci : public SignalBase
{
public:
    constexpr pci(Response &r) : SignalBase(r, 12, 4, kEncoding_pci) {}
};
class sid : public SignalBase
{
public:
    constexpr sid(Response &r) : SignalBase(r, 16, 8, kEncoding_service_id) {}
};
class d1 : public SignalBase
{
public:
    constexpr d1(Response &r) : SignalBase(r, 24, 8, kEncoding_none) {}
};
class d2 : public SignalBase
{
public:
    constexpr d2(Response &r) : SignalBase(r, 32, 8, kEncoding_none) {}
};
class d3 : public SignalBase
{
public:
    constexpr d3(Response &r) : SignalBase(r, 40, 8, kEncoding_none) {}
};
class d4 : public SignalBase
{
public:
    constexpr d4(Response &r) : SignalBase(r, 48, 8, kEncoding_none) {}
};
class d5 : public SignalBase
{
public:
    constexpr d5(Response &r) : SignalBase(r, 56, 8, kEncoding_none) {}
};
class vendor : public SignalBase
{
public:
    constexpr vendor(Response &r) : SignalBase(r, 24, 16, kEncoding_none) {}
};
class function : public SignalBase
{
public:
    constexpr function(Response &r) : SignalBase(r, 40, 16, kEncoding_none) {}
};
class variant : public SignalBase
{
public:
    constexpr variant(Response &r) : SignalBase(r, 56, 8, kEncoding_none) {}
};
class index : public SignalBase
{
public:
    constexpr index(Response &r) : SignalBase(r, 24, 16, kEncoding_none) {}
};
class value : public SignalBase
{
public:
    constexpr value(Response &r) : SignalBase(r, 40, 16, kEncoding_none) {}
};
class Ignition : public SignalBase
{
public:
    constexpr Ignition(Response &r) : SignalBase(r, 0, 1, kEncoding_none) {}
};
class Start : public SignalBase
{
public:
    constexpr Start(Response &r) : SignalBase(r, 1, 1, kEncoding_none) {}
};
class LightsUp : public SignalBase
{
public:
    constexpr LightsUp(Response &r) : SignalBase(r, 2, 1, kEncoding_none) {}
};
class LightsDown : public SignalBase
{
public:
    constexpr LightsDown(Response &r) : SignalBase(r, 3, 1, kEncoding_none) {}
};
class HeadLights : public SignalBase
{
public:
    constexpr HeadLights(Response &r) : SignalBase(r, 4, 1, kEncoding_none) {}
};
class LowBeam : public SignalBase
{
public:
    constexpr LowBeam(Response &r) : SignalBase(r, 5, 1, kEncoding_none) {}
};
class HighBeam : public SignalBase
{
public:
    constexpr HighBeam(Response &r) : SignalBase(r, 6, 1, kEncoding_none) {}
};
class FogLights : public SignalBase
{
public:
    constexpr FogLights(Response &r) : SignalBase(r, 7, 1, kEncoding_none) {}
};
class MarkerLights : public SignalBase
{
public:
    constexpr MarkerLights(Response &r) : SignalBase(r, 8, 1, kEncoding_none) {}
};
class LeftTurn : public SignalBase
{
public:
    constexpr LeftTurn(Response &r) : SignalBase(r, 9, 1, kEncoding_none) {}
};
class LeftTurnMarker : public SignalBase
{
public:
    constexpr LeftTurnMarker(Response &r) : SignalBase(r, 10, 1, kEncoding_none) {}
};
class RightTurn : public SignalBase
{
public:
    constexpr RightTurn(Response &r) : SignalBase(r, 11, 1, kEncoding_none) {}
};
class RightTurnMarker : public SignalBase
{
public:
    constexpr RightTurnMarker(Response &r) : SignalBase(r, 12, 1, kEncoding_none) {}
};
class Brake : public SignalBase
{
public:
    constexpr Brake(Response &r) : SignalBase(r, 13, 1, kEncoding_none) {}
};
class Reverse : public SignalBase
{
public:
    constexpr Reverse(Response &r) : SignalBase(r, 14, 1, kEncoding_none) {}
};
class InteriorLight : public SignalBase
{
public:
    constexpr InteriorLight(Response &r) : SignalBase(r, 15, 1, kEncoding_none) {}
};
class CabinFan1 : public SignalBase
{
public:
    constexpr CabinFan1(Response &r) : SignalBase(r, 16, 1, kEncoding_none) {}
};
class CabinFan2 : public SignalBase
{
public:
    constexpr CabinFan2(Response &r) : SignalBase(r, 17, 1, kEncoding_none) {}
};
class CabinFan3 : public SignalBase
{
public:
    constexpr CabinFan3(Response &r) : SignalBase(r, 18, 1, kEncoding_none) {}
};
class CabinFan4 : public SignalBase
{
public:
    constexpr CabinFan4(Response &r) : SignalBase(r, 19, 1, kEncoding_none) {}
};
class WiperLow : public SignalBase
{
public:
    constexpr WiperLow(Response &r) : SignalBase(r, 20, 1, kEncoding_none) {}
};
class WiperHigh : public SignalBase
{
public:
    constexpr WiperHigh(Response &r) : SignalBase(r, 21, 1, kEncoding_none) {}
};
class RearDefrost : public SignalBase
{
public:
    constexpr RearDefrost(Response &r) : SignalBase(r, 22, 1, kEncoding_none) {}
};
class RPM : public SignalBase
{
public:
    constexpr RPM(Response &r) : SignalBase(r, 0, 8, kEncoding_RPM) {}
};
class oilPressure : public SignalBase
{
public:
    constexpr oilPressure(Response &r) : SignalBase(r, 8, 8, kEncoding_PSI) {}
};
class oilTemperature : public SignalBase
{
public:
    constexpr oilTemperature(Response &r) : SignalBase(r, 16, 8, kEncoding_F) {}
};
class coolantTemperature : public SignalBase
{
public:
    constexpr coolantTemperature(Response &r) : SignalBase(r, 24, 8, kEncoding_F) {}
};
class fuelPressure : public SignalBase
{
public:
    constexpr fuelPressure(Response &r) : SignalBase(r, 32, 8, kEncoding_PSI) {}
};
class voltage : public SignalBase
{
public:
    constexpr voltage(Response &r) : SignalBase(r, 40, 8, kEncoding_V) {}
};
class AFR : public SignalBase
{
public:
    constexpr AFR(Response &r) : SignalBase(r, 48, 8, kEncoding_lambda) {}
};
class roadSpeed : public SignalBase
{
public:
    constexpr roadSpeed(Response &r) : SignalBase(r, 56, 8, kEncoding_MPH) {}
};
} // namespace Signal
static const uint8_t kFrameIDSlaveResponse = 0x3d;
static const uint8_t kFrameIDRelays = 0x01;
static const uint8_t kFrameIDMasterRequest = 0x3c;
static const uint8_t kFrameIDECUData = 0x02;
static const uint8_t kFrameIDProxyRequest = 0x3b;

namespace Generic
{
extern void param_default(const Parameter &param);
extern const PROGMEM char *param_name(const Parameter &param);
extern Parameter parameter(Parameter::Address address);

static const uint16_t kParamProtocolVersion = 0x0000;
static const uint16_t kParamBoardFunction = 0x0001;
static const uint16_t kParamBootloaderMode = 0x0002;
static const uint16_t kParamFirmwareVersion = 0x0003;
static const uint16_t kParamFirmwarePageSize = 0x0004;
static const uint16_t kParamWatchdogResets = 0x0100;
static const uint16_t kParamSupplyVoltage = 0x0101;
static const uint16_t kParamTemperature = 0x0102;
static const uint16_t kParamLine = 0x0200;
static const uint16_t kParamChecksum = 0x0201;
static const uint16_t kParamParity = 0x0202;
static const uint16_t kParamFraming = 0x0203;
static const uint16_t kParamSynch = 0x0204;
static const uint16_t kParamProtocol = 0x0205;
static const uint16_t kParamConfigBase = 0x0400;

// explicit parameter subclasses for Generic

class ParamProtocolVersion : public Parameter
{
public:
    constexpr ParamProtocolVersion() : Parameter(kParamProtocolVersion, kEncoding_none, param_default, param_name) {}
};
static const ParamProtocolVersion paramProtocolVersion;

class ParamBoardFunction : public Parameter
{
public:
    constexpr ParamBoardFunction() : Parameter(kParamBoardFunction, kEncoding_none, param_default, param_name) {}
};
static const ParamBoardFunction paramBoardFunction;

class ParamBootloaderMode : public Parameter
{
public:
    constexpr ParamBootloaderMode() : Parameter(kParamBootloaderMode, kEncoding_none, param_default, param_name) {}
};
static const ParamBootloaderMode paramBootloaderMode;

class ParamFirmwareVersion : public Parameter
{
public:
    constexpr ParamFirmwareVersion() : Parameter(kParamFirmwareVersion, kEncoding_none, param_default, param_name) {}
};
static const ParamFirmwareVersion paramFirmwareVersion;

class ParamFirmwarePageSize : public Parameter
{
public:
    constexpr ParamFirmwarePageSize() : Parameter(kParamFirmwarePageSize, kEncoding_none, param_default, param_name) {}
};
static const ParamFirmwarePageSize paramFirmwarePageSize;

class ParamWatchdogResets : public Parameter
{
public:
    constexpr ParamWatchdogResets() : Parameter(kParamWatchdogResets, kEncoding_none, param_default, param_name) {}
};
static const ParamWatchdogResets paramWatchdogResets;

class ParamSupplyVoltage : public Parameter
{
public:
    constexpr ParamSupplyVoltage() : Parameter(kParamSupplyVoltage, kEncoding_none, param_default, param_name) {}
};
static const ParamSupplyVoltage paramSupplyVoltage;

class ParamTemperature : public Parameter
{
public:
    constexpr ParamTemperature() : Parameter(kParamTemperature, kEncoding_none, param_default, param_name) {}
};
static const ParamTemperature paramTemperature;

class ParamLine : public Parameter
{
public:
    constexpr ParamLine() : Parameter(kParamLine, kEncoding_none, param_default, param_name) {}
};
static const ParamLine paramLine;

class ParamChecksum : public Parameter
{
public:
    constexpr ParamChecksum() : Parameter(kParamChecksum, kEncoding_none, param_default, param_name) {}
};
static const ParamChecksum paramChecksum;

class ParamParity : public Parameter
{
public:
    constexpr ParamParity() : Parameter(kParamParity, kEncoding_none, param_default, param_name) {}
};
static const ParamParity paramParity;

class ParamFraming : public Parameter
{
public:
    constexpr ParamFraming() : Parameter(kParamFraming, kEncoding_none, param_default, param_name) {}
};
static const ParamFraming paramFraming;

class ParamSynch : public Parameter
{
public:
    constexpr ParamSynch() : Parameter(kParamSynch, kEncoding_none, param_default, param_name) {}
};
static const ParamSynch paramSynch;

class ParamProtocol : public Parameter
{
public:
    constexpr ParamProtocol() : Parameter(kParamProtocol, kEncoding_none, param_default, param_name) {}
};
static const ParamProtocol paramProtocol;

class ParamConfigBase : public Parameter
{
public:
    constexpr ParamConfigBase() : Parameter(kParamConfigBase, kEncoding_none, param_default, param_name) {}
};
static const ParamConfigBase paramConfigBase;

} // namespace Generic

namespace Master
{
extern void param_default(const Parameter &param);
extern const PROGMEM char *param_name(const Parameter &param);
extern Parameter parameter(Parameter::Address address);

static const uint16_t kParamSP1Assign = 0x0400;
static const uint16_t kParamSP2Assign = 0x0401;
static const uint16_t kParamSP3Assign = 0x0402;
static const uint16_t kParamSP4Assign = 0x0403;
static const uint16_t kParamSP5Assign = 0x0404;
static const uint16_t kParamSP6Assign = 0x0405;
static const uint16_t kParamSP7Assign = 0x0406;
static const uint16_t kParamSG0Assign = 0x0407;
static const uint16_t kParamSG1Assign = 0x0408;
static const uint16_t kParamSG2Assign = 0x0409;
static const uint16_t kParamSG3Assign = 0x040a;
static const uint16_t kParamSG4Assign = 0x040b;
static const uint16_t kParamSG5Assign = 0x040c;
static const uint16_t kParamSG6Assign = 0x040d;
static const uint16_t kParamSG7Assign = 0x040e;
static const uint16_t kParamSG8Assign = 0x040f;
static const uint16_t kParamSG9Assign = 0x0410;
static const uint16_t kParamSG10Assign = 0x0411;
static const uint16_t kParamSG11Assign = 0x0412;
static const uint16_t kParamSG12Assign = 0x0413;
static const uint16_t kParamSG13Assign = 0x0414;
static const uint16_t kParamTurnBlinkPeriod = 0x0415;
static const uint16_t kParamPassingBlinkCount = 0x0416;
static const uint16_t kParamPathLightTime = 0x0417;
static const uint16_t kParamInteriorLightTime = 0x0418;
static const uint16_t kParamWelcomeLightTime = 0x0419;
static const uint16_t kParamBrakeBlinkPeriod = 0x041a;
static const uint16_t kParamBrakeBlinkCount = 0x041b;
static const uint16_t kParamWiperInterval = 0x041c;

// explicit parameter subclasses for Master

class ParamSP1Assign : public Parameter
{
public:
    constexpr ParamSP1Assign() : Parameter(kParamSP1Assign, kEncoding_input, param_default, param_name) {}
};
static const ParamSP1Assign paramSP1Assign;

class ParamSP2Assign : public Parameter
{
public:
    constexpr ParamSP2Assign() : Parameter(kParamSP2Assign, kEncoding_input, param_default, param_name) {}
};
static const ParamSP2Assign paramSP2Assign;

class ParamSP3Assign : public Parameter
{
public:
    constexpr ParamSP3Assign() : Parameter(kParamSP3Assign, kEncoding_input, param_default, param_name) {}
};
static const ParamSP3Assign paramSP3Assign;

class ParamSP4Assign : public Parameter
{
public:
    constexpr ParamSP4Assign() : Parameter(kParamSP4Assign, kEncoding_input, param_default, param_name) {}
};
static const ParamSP4Assign paramSP4Assign;

class ParamSP5Assign : public Parameter
{
public:
    constexpr ParamSP5Assign() : Parameter(kParamSP5Assign, kEncoding_input, param_default, param_name) {}
};
static const ParamSP5Assign paramSP5Assign;

class ParamSP6Assign : public Parameter
{
public:
    constexpr ParamSP6Assign() : Parameter(kParamSP6Assign, kEncoding_input, param_default, param_name) {}
};
static const ParamSP6Assign paramSP6Assign;

class ParamSP7Assign : public Parameter
{
public:
    constexpr ParamSP7Assign() : Parameter(kParamSP7Assign, kEncoding_input, param_default, param_name) {}
};
static const ParamSP7Assign paramSP7Assign;

class ParamSG0Assign : public Parameter
{
public:
    constexpr ParamSG0Assign() : Parameter(kParamSG0Assign, kEncoding_input, param_default, param_name) {}
};
static const ParamSG0Assign paramSG0Assign;

class ParamSG1Assign : public Parameter
{
public:
    constexpr ParamSG1Assign() : Parameter(kParamSG1Assign, kEncoding_input, param_default, param_name) {}
};
static const ParamSG1Assign paramSG1Assign;

class ParamSG2Assign : public Parameter
{
public:
    constexpr ParamSG2Assign() : Parameter(kParamSG2Assign, kEncoding_input, param_default, param_name) {}
};
static const ParamSG2Assign paramSG2Assign;

class ParamSG3Assign : public Parameter
{
public:
    constexpr ParamSG3Assign() : Parameter(kParamSG3Assign, kEncoding_input, param_default, param_name) {}
};
static const ParamSG3Assign paramSG3Assign;

class ParamSG4Assign : public Parameter
{
public:
    constexpr ParamSG4Assign() : Parameter(kParamSG4Assign, kEncoding_input, param_default, param_name) {}
};
static const ParamSG4Assign paramSG4Assign;

class ParamSG5Assign : public Parameter
{
public:
    constexpr ParamSG5Assign() : Parameter(kParamSG5Assign, kEncoding_input, param_default, param_name) {}
};
static const ParamSG5Assign paramSG5Assign;

class ParamSG6Assign : public Parameter
{
public:
    constexpr ParamSG6Assign() : Parameter(kParamSG6Assign, kEncoding_input, param_default, param_name) {}
};
static const ParamSG6Assign paramSG6Assign;

class ParamSG7Assign : public Parameter
{
public:
    constexpr ParamSG7Assign() : Parameter(kParamSG7Assign, kEncoding_input, param_default, param_name) {}
};
static const ParamSG7Assign paramSG7Assign;

class ParamSG8Assign : public Parameter
{
public:
    constexpr ParamSG8Assign() : Parameter(kParamSG8Assign, kEncoding_input, param_default, param_name) {}
};
static const ParamSG8Assign paramSG8Assign;

class ParamSG9Assign : public Parameter
{
public:
    constexpr ParamSG9Assign() : Parameter(kParamSG9Assign, kEncoding_input, param_default, param_name) {}
};
static const ParamSG9Assign paramSG9Assign;

class ParamSG10Assign : public Parameter
{
public:
    constexpr ParamSG10Assign() : Parameter(kParamSG10Assign, kEncoding_input, param_default, param_name) {}
};
static const ParamSG10Assign paramSG10Assign;

class ParamSG11Assign : public Parameter
{
public:
    constexpr ParamSG11Assign() : Parameter(kParamSG11Assign, kEncoding_input, param_default, param_name) {}
};
static const ParamSG11Assign paramSG11Assign;

class ParamSG12Assign : public Parameter
{
public:
    constexpr ParamSG12Assign() : Parameter(kParamSG12Assign, kEncoding_input, param_default, param_name) {}
};
static const ParamSG12Assign paramSG12Assign;

class ParamSG13Assign : public Parameter
{
public:
    constexpr ParamSG13Assign() : Parameter(kParamSG13Assign, kEncoding_input, param_default, param_name) {}
};
static const ParamSG13Assign paramSG13Assign;

class ParamTurnBlinkPeriod : public Parameter
{
public:
    constexpr ParamTurnBlinkPeriod() : Parameter(kParamTurnBlinkPeriod, kEncoding_msec, param_default, param_name) {}
};
static const ParamTurnBlinkPeriod paramTurnBlinkPeriod;

class ParamPassingBlinkCount : public Parameter
{
public:
    constexpr ParamPassingBlinkCount() : Parameter(kParamPassingBlinkCount, kEncoding_up_to_10, param_default, param_name) {}
};
static const ParamPassingBlinkCount paramPassingBlinkCount;

class ParamPathLightTime : public Parameter
{
public:
    constexpr ParamPathLightTime() : Parameter(kParamPathLightTime, kEncoding_sec, param_default, param_name) {}
};
static const ParamPathLightTime paramPathLightTime;

class ParamInteriorLightTime : public Parameter
{
public:
    constexpr ParamInteriorLightTime() : Parameter(kParamInteriorLightTime, kEncoding_sec, param_default, param_name) {}
};
static const ParamInteriorLightTime paramInteriorLightTime;

class ParamWelcomeLightTime : public Parameter
{
public:
    constexpr ParamWelcomeLightTime() : Parameter(kParamWelcomeLightTime, kEncoding_sec, param_default, param_name) {}
};
static const ParamWelcomeLightTime paramWelcomeLightTime;

class ParamBrakeBlinkPeriod : public Parameter
{
public:
    constexpr ParamBrakeBlinkPeriod() : Parameter(kParamBrakeBlinkPeriod, kEncoding_msec, param_default, param_name) {}
};
static const ParamBrakeBlinkPeriod paramBrakeBlinkPeriod;

class ParamBrakeBlinkCount : public Parameter
{
public:
    constexpr ParamBrakeBlinkCount() : Parameter(kParamBrakeBlinkCount, kEncoding_up_to_10, param_default, param_name) {}
};
static const ParamBrakeBlinkCount paramBrakeBlinkCount;

class ParamWiperInterval : public Parameter
{
public:
    constexpr ParamWiperInterval() : Parameter(kParamWiperInterval, kEncoding_msec, param_default, param_name) {}
};
static const ParamWiperInterval paramWiperInterval;

} // namespace Master

namespace PowerV1
{
extern void param_default(const Parameter &param);
extern const PROGMEM char *param_name(const Parameter &param);
extern Parameter parameter(Parameter::Address address);

static const uint16_t kParamRelay1Status = 0x0300;
static const uint16_t kParamRelay2Status = 0x0301;
static const uint16_t kParamRelay3Status = 0x0302;
static const uint16_t kParamRelay4Status = 0x0303;
static const uint16_t kParamRelay1Assign = 0x0400;
static const uint16_t kParamRelay2Assign = 0x0401;
static const uint16_t kParamRelay3Assign = 0x0402;
static const uint16_t kParamRelay4Assign = 0x0403;

// explicit parameter subclasses for PowerV1

class ParamRelay1Status : public Parameter
{
public:
    constexpr ParamRelay1Status() : Parameter(kParamRelay1Status, kEncoding_v1_output_status, param_default, param_name) {}
};
static const ParamRelay1Status paramRelay1Status;

class ParamRelay2Status : public Parameter
{
public:
    constexpr ParamRelay2Status() : Parameter(kParamRelay2Status, kEncoding_v1_output_status, param_default, param_name) {}
};
static const ParamRelay2Status paramRelay2Status;

class ParamRelay3Status : public Parameter
{
public:
    constexpr ParamRelay3Status() : Parameter(kParamRelay3Status, kEncoding_v1_output_status, param_default, param_name) {}
};
static const ParamRelay3Status paramRelay3Status;

class ParamRelay4Status : public Parameter
{
public:
    constexpr ParamRelay4Status() : Parameter(kParamRelay4Status, kEncoding_v1_output_status, param_default, param_name) {}
};
static const ParamRelay4Status paramRelay4Status;

class ParamRelay1Assign : public Parameter
{
public:
    constexpr ParamRelay1Assign() : Parameter(kParamRelay1Assign, kEncoding_v1_output_assignment, param_default, param_name) {}
};
static const ParamRelay1Assign paramRelay1Assign;

class ParamRelay2Assign : public Parameter
{
public:
    constexpr ParamRelay2Assign() : Parameter(kParamRelay2Assign, kEncoding_v1_output_assignment, param_default, param_name) {}
};
static const ParamRelay2Assign paramRelay2Assign;

class ParamRelay3Assign : public Parameter
{
public:
    constexpr ParamRelay3Assign() : Parameter(kParamRelay3Assign, kEncoding_v1_output_assignment, param_default, param_name) {}
};
static const ParamRelay3Assign paramRelay3Assign;

class ParamRelay4Assign : public Parameter
{
public:
    constexpr ParamRelay4Assign() : Parameter(kParamRelay4Assign, kEncoding_v1_output_assignment, param_default, param_name) {}
};
static const ParamRelay4Assign paramRelay4Assign;

} // namespace PowerV1

namespace PowerV3
{
extern void param_default(const Parameter &param);
extern const PROGMEM char *param_name(const Parameter &param);
extern Parameter parameter(Parameter::Address address);

static const uint16_t kParamCH1Status = 0x0300;
static const uint16_t kParamCH2Status = 0x0301;
static const uint16_t kParamCH3Status = 0x0302;
static const uint16_t kParamCH4Status = 0x0303;
static const uint16_t kParamCH5Status = 0x0304;
static const uint16_t kParamCH1Type = 0x0400;
static const uint16_t kParamCH2Type = 0x0401;
static const uint16_t kParamCH3Type = 0x0402;
static const uint16_t kParamCH4Type = 0x0403;
static const uint16_t kParamCH5Type = 0x0404;
static const uint16_t kParamCH1Assign1 = 0x0405;
static const uint16_t kParamCH1Assign2 = 0x0406;
static const uint16_t kParamCH1Assign3 = 0x0407;
static const uint16_t kParamCH1Assign4 = 0x0408;
static const uint16_t kParamCH1PWM1 = 0x0409;
static const uint16_t kParamCH1PWM2 = 0x040a;
static const uint16_t kParamCH1PWM3 = 0x040b;
static const uint16_t kParamCH1PWM4 = 0x040c;
static const uint16_t kParamCH2Assign1 = 0x040d;
static const uint16_t kParamCH2Assign2 = 0x040e;
static const uint16_t kParamCH2Assign3 = 0x040f;
static const uint16_t kParamCH2Assign4 = 0x0410;
static const uint16_t kParamCH2PWM1 = 0x0411;
static const uint16_t kParamCH2PWM2 = 0x0412;
static const uint16_t kParamCH2PWM3 = 0x0413;
static const uint16_t kParamCH2PWM4 = 0x0414;
static const uint16_t kParamCH3Assign1 = 0x0415;
static const uint16_t kParamCH3Assign2 = 0x0416;
static const uint16_t kParamCH3Assign3 = 0x0417;
static const uint16_t kParamCH3Assign4 = 0x0418;
static const uint16_t kParamCH3PWM1 = 0x0419;
static const uint16_t kParamCH3PWM2 = 0x041a;
static const uint16_t kParamCH3PWM3 = 0x041b;
static const uint16_t kParamCH3PWM4 = 0x041c;
static const uint16_t kParamCH4Assign1 = 0x041d;
static const uint16_t kParamCH4Assign2 = 0x041e;
static const uint16_t kParamCH4Assign3 = 0x041f;
static const uint16_t kParamCH4Assign4 = 0x0420;
static const uint16_t kParamCH4PWM1 = 0x0421;
static const uint16_t kParamCH4PWM2 = 0x0422;
static const uint16_t kParamCH4PWM3 = 0x0423;
static const uint16_t kParamCH4PWM4 = 0x0424;
static const uint16_t kParamCH5Assign1 = 0x0425;
static const uint16_t kParamCH5Assign2 = 0x0426;
static const uint16_t kParamCH5Assign3 = 0x0427;
static const uint16_t kParamCH5Assign4 = 0x0428;
static const uint16_t kParamCH5PWM1 = 0x0429;
static const uint16_t kParamCH5PWM2 = 0x042a;
static const uint16_t kParamCH5PWM3 = 0x042b;
static const uint16_t kParamCH5PWM4 = 0x042c;

// explicit parameter subclasses for PowerV3

class ParamCH1Status : public Parameter
{
public:
    constexpr ParamCH1Status() : Parameter(kParamCH1Status, kEncoding_v3_output_status, param_default, param_name) {}
};
static const ParamCH1Status paramCH1Status;

class ParamCH2Status : public Parameter
{
public:
    constexpr ParamCH2Status() : Parameter(kParamCH2Status, kEncoding_v3_output_status, param_default, param_name) {}
};
static const ParamCH2Status paramCH2Status;

class ParamCH3Status : public Parameter
{
public:
    constexpr ParamCH3Status() : Parameter(kParamCH3Status, kEncoding_v3_output_status, param_default, param_name) {}
};
static const ParamCH3Status paramCH3Status;

class ParamCH4Status : public Parameter
{
public:
    constexpr ParamCH4Status() : Parameter(kParamCH4Status, kEncoding_v3_output_status, param_default, param_name) {}
};
static const ParamCH4Status paramCH4Status;

class ParamCH5Status : public Parameter
{
public:
    constexpr ParamCH5Status() : Parameter(kParamCH5Status, kEncoding_v3_output_status, param_default, param_name) {}
};
static const ParamCH5Status paramCH5Status;

class ParamCH1Type : public Parameter
{
public:
    constexpr ParamCH1Type() : Parameter(kParamCH1Type, kEncoding_output_type, param_default, param_name) {}
};
static const ParamCH1Type paramCH1Type;

class ParamCH2Type : public Parameter
{
public:
    constexpr ParamCH2Type() : Parameter(kParamCH2Type, kEncoding_output_type, param_default, param_name) {}
};
static const ParamCH2Type paramCH2Type;

class ParamCH3Type : public Parameter
{
public:
    constexpr ParamCH3Type() : Parameter(kParamCH3Type, kEncoding_output_type, param_default, param_name) {}
};
static const ParamCH3Type paramCH3Type;

class ParamCH4Type : public Parameter
{
public:
    constexpr ParamCH4Type() : Parameter(kParamCH4Type, kEncoding_output_type, param_default, param_name) {}
};
static const ParamCH4Type paramCH4Type;

class ParamCH5Type : public Parameter
{
public:
    constexpr ParamCH5Type() : Parameter(kParamCH5Type, kEncoding_output_type, param_default, param_name) {}
};
static const ParamCH5Type paramCH5Type;

class ParamCH1Assign1 : public Parameter
{
public:
    constexpr ParamCH1Assign1() : Parameter(kParamCH1Assign1, kEncoding_output_assignment, param_default, param_name) {}
};
static const ParamCH1Assign1 paramCH1Assign1;

class ParamCH1Assign2 : public Parameter
{
public:
    constexpr ParamCH1Assign2() : Parameter(kParamCH1Assign2, kEncoding_output_assignment, param_default, param_name) {}
};
static const ParamCH1Assign2 paramCH1Assign2;

class ParamCH1Assign3 : public Parameter
{
public:
    constexpr ParamCH1Assign3() : Parameter(kParamCH1Assign3, kEncoding_output_assignment, param_default, param_name) {}
};
static const ParamCH1Assign3 paramCH1Assign3;

class ParamCH1Assign4 : public Parameter
{
public:
    constexpr ParamCH1Assign4() : Parameter(kParamCH1Assign4, kEncoding_output_assignment, param_default, param_name) {}
};
static const ParamCH1Assign4 paramCH1Assign4;

class ParamCH1PWM1 : public Parameter
{
public:
    constexpr ParamCH1PWM1() : Parameter(kParamCH1PWM1, kEncoding_pwm_duty_cycle, param_default, param_name) {}
};
static const ParamCH1PWM1 paramCH1PWM1;

class ParamCH1PWM2 : public Parameter
{
public:
    constexpr ParamCH1PWM2() : Parameter(kParamCH1PWM2, kEncoding_pwm_duty_cycle, param_default, param_name) {}
};
static const ParamCH1PWM2 paramCH1PWM2;

class ParamCH1PWM3 : public Parameter
{
public:
    constexpr ParamCH1PWM3() : Parameter(kParamCH1PWM3, kEncoding_pwm_duty_cycle, param_default, param_name) {}
};
static const ParamCH1PWM3 paramCH1PWM3;

class ParamCH1PWM4 : public Parameter
{
public:
    constexpr ParamCH1PWM4() : Parameter(kParamCH1PWM4, kEncoding_pwm_duty_cycle, param_default, param_name) {}
};
static const ParamCH1PWM4 paramCH1PWM4;

class ParamCH2Assign1 : public Parameter
{
public:
    constexpr ParamCH2Assign1() : Parameter(kParamCH2Assign1, kEncoding_output_assignment, param_default, param_name) {}
};
static const ParamCH2Assign1 paramCH2Assign1;

class ParamCH2Assign2 : public Parameter
{
public:
    constexpr ParamCH2Assign2() : Parameter(kParamCH2Assign2, kEncoding_output_assignment, param_default, param_name) {}
};
static const ParamCH2Assign2 paramCH2Assign2;

class ParamCH2Assign3 : public Parameter
{
public:
    constexpr ParamCH2Assign3() : Parameter(kParamCH2Assign3, kEncoding_output_assignment, param_default, param_name) {}
};
static const ParamCH2Assign3 paramCH2Assign3;

class ParamCH2Assign4 : public Parameter
{
public:
    constexpr ParamCH2Assign4() : Parameter(kParamCH2Assign4, kEncoding_output_assignment, param_default, param_name) {}
};
static const ParamCH2Assign4 paramCH2Assign4;

class ParamCH2PWM1 : public Parameter
{
public:
    constexpr ParamCH2PWM1() : Parameter(kParamCH2PWM1, kEncoding_pwm_duty_cycle, param_default, param_name) {}
};
static const ParamCH2PWM1 paramCH2PWM1;

class ParamCH2PWM2 : public Parameter
{
public:
    constexpr ParamCH2PWM2() : Parameter(kParamCH2PWM2, kEncoding_pwm_duty_cycle, param_default, param_name) {}
};
static const ParamCH2PWM2 paramCH2PWM2;

class ParamCH2PWM3 : public Parameter
{
public:
    constexpr ParamCH2PWM3() : Parameter(kParamCH2PWM3, kEncoding_pwm_duty_cycle, param_default, param_name) {}
};
static const ParamCH2PWM3 paramCH2PWM3;

class ParamCH2PWM4 : public Parameter
{
public:
    constexpr ParamCH2PWM4() : Parameter(kParamCH2PWM4, kEncoding_pwm_duty_cycle, param_default, param_name) {}
};
static const ParamCH2PWM4 paramCH2PWM4;

class ParamCH3Assign1 : public Parameter
{
public:
    constexpr ParamCH3Assign1() : Parameter(kParamCH3Assign1, kEncoding_output_assignment, param_default, param_name) {}
};
static const ParamCH3Assign1 paramCH3Assign1;

class ParamCH3Assign2 : public Parameter
{
public:
    constexpr ParamCH3Assign2() : Parameter(kParamCH3Assign2, kEncoding_output_assignment, param_default, param_name) {}
};
static const ParamCH3Assign2 paramCH3Assign2;

class ParamCH3Assign3 : public Parameter
{
public:
    constexpr ParamCH3Assign3() : Parameter(kParamCH3Assign3, kEncoding_output_assignment, param_default, param_name) {}
};
static const ParamCH3Assign3 paramCH3Assign3;

class ParamCH3Assign4 : public Parameter
{
public:
    constexpr ParamCH3Assign4() : Parameter(kParamCH3Assign4, kEncoding_output_assignment, param_default, param_name) {}
};
static const ParamCH3Assign4 paramCH3Assign4;

class ParamCH3PWM1 : public Parameter
{
public:
    constexpr ParamCH3PWM1() : Parameter(kParamCH3PWM1, kEncoding_pwm_duty_cycle, param_default, param_name) {}
};
static const ParamCH3PWM1 paramCH3PWM1;

class ParamCH3PWM2 : public Parameter
{
public:
    constexpr ParamCH3PWM2() : Parameter(kParamCH3PWM2, kEncoding_pwm_duty_cycle, param_default, param_name) {}
};
static const ParamCH3PWM2 paramCH3PWM2;

class ParamCH3PWM3 : public Parameter
{
public:
    constexpr ParamCH3PWM3() : Parameter(kParamCH3PWM3, kEncoding_pwm_duty_cycle, param_default, param_name) {}
};
static const ParamCH3PWM3 paramCH3PWM3;

class ParamCH3PWM4 : public Parameter
{
public:
    constexpr ParamCH3PWM4() : Parameter(kParamCH3PWM4, kEncoding_pwm_duty_cycle, param_default, param_name) {}
};
static const ParamCH3PWM4 paramCH3PWM4;

class ParamCH4Assign1 : public Parameter
{
public:
    constexpr ParamCH4Assign1() : Parameter(kParamCH4Assign1, kEncoding_output_assignment, param_default, param_name) {}
};
static const ParamCH4Assign1 paramCH4Assign1;

class ParamCH4Assign2 : public Parameter
{
public:
    constexpr ParamCH4Assign2() : Parameter(kParamCH4Assign2, kEncoding_output_assignment, param_default, param_name) {}
};
static const ParamCH4Assign2 paramCH4Assign2;

class ParamCH4Assign3 : public Parameter
{
public:
    constexpr ParamCH4Assign3() : Parameter(kParamCH4Assign3, kEncoding_output_assignment, param_default, param_name) {}
};
static const ParamCH4Assign3 paramCH4Assign3;

class ParamCH4Assign4 : public Parameter
{
public:
    constexpr ParamCH4Assign4() : Parameter(kParamCH4Assign4, kEncoding_output_assignment, param_default, param_name) {}
};
static const ParamCH4Assign4 paramCH4Assign4;

class ParamCH4PWM1 : public Parameter
{
public:
    constexpr ParamCH4PWM1() : Parameter(kParamCH4PWM1, kEncoding_pwm_duty_cycle, param_default, param_name) {}
};
static const ParamCH4PWM1 paramCH4PWM1;

class ParamCH4PWM2 : public Parameter
{
public:
    constexpr ParamCH4PWM2() : Parameter(kParamCH4PWM2, kEncoding_pwm_duty_cycle, param_default, param_name) {}
};
static const ParamCH4PWM2 paramCH4PWM2;

class ParamCH4PWM3 : public Parameter
{
public:
    constexpr ParamCH4PWM3() : Parameter(kParamCH4PWM3, kEncoding_pwm_duty_cycle, param_default, param_name) {}
};
static const ParamCH4PWM3 paramCH4PWM3;

class ParamCH4PWM4 : public Parameter
{
public:
    constexpr ParamCH4PWM4() : Parameter(kParamCH4PWM4, kEncoding_pwm_duty_cycle, param_default, param_name) {}
};
static const ParamCH4PWM4 paramCH4PWM4;

class ParamCH5Assign1 : public Parameter
{
public:
    constexpr ParamCH5Assign1() : Parameter(kParamCH5Assign1, kEncoding_output_assignment, param_default, param_name) {}
};
static const ParamCH5Assign1 paramCH5Assign1;

class ParamCH5Assign2 : public Parameter
{
public:
    constexpr ParamCH5Assign2() : Parameter(kParamCH5Assign2, kEncoding_output_assignment, param_default, param_name) {}
};
static const ParamCH5Assign2 paramCH5Assign2;

class ParamCH5Assign3 : public Parameter
{
public:
    constexpr ParamCH5Assign3() : Parameter(kParamCH5Assign3, kEncoding_output_assignment, param_default, param_name) {}
};
static const ParamCH5Assign3 paramCH5Assign3;

class ParamCH5Assign4 : public Parameter
{
public:
    constexpr ParamCH5Assign4() : Parameter(kParamCH5Assign4, kEncoding_output_assignment, param_default, param_name) {}
};
static const ParamCH5Assign4 paramCH5Assign4;

class ParamCH5PWM1 : public Parameter
{
public:
    constexpr ParamCH5PWM1() : Parameter(kParamCH5PWM1, kEncoding_pwm_duty_cycle, param_default, param_name) {}
};
static const ParamCH5PWM1 paramCH5PWM1;

class ParamCH5PWM2 : public Parameter
{
public:
    constexpr ParamCH5PWM2() : Parameter(kParamCH5PWM2, kEncoding_pwm_duty_cycle, param_default, param_name) {}
};
static const ParamCH5PWM2 paramCH5PWM2;

class ParamCH5PWM3 : public Parameter
{
public:
    constexpr ParamCH5PWM3() : Parameter(kParamCH5PWM3, kEncoding_pwm_duty_cycle, param_default, param_name) {}
};
static const ParamCH5PWM3 paramCH5PWM3;

class ParamCH5PWM4 : public Parameter
{
public:
    constexpr ParamCH5PWM4() : Parameter(kParamCH5PWM4, kEncoding_pwm_duty_cycle, param_default, param_name) {}
};
static const ParamCH5PWM4 paramCH5PWM4;

} // namespace PowerV3

namespace Generic
{
    static const uint8_t kNodeAddress  = 1;
    static const uint16_t kNodeSupplier = 0xb007;
    static const uint16_t kNodeFunction = 0x0000;
    static const uint8_t kNodeVariant  = 0x00;
} // namespace Generic

namespace Master
{
    static const uint8_t kNodeAddress  = 1;
    static const uint16_t kNodeSupplier = 0xb007;
    static const uint16_t kNodeFunction = 0x0000;
    static const uint8_t kNodeVariant  = 0x01;
} // namespace Master

namespace PowerV1
{
    static const uint8_t kNodeAddress  = 2;
    static const uint16_t kNodeSupplier = 0xb007;
    static const uint16_t kNodeFunction = 0x0001;
    static const uint8_t kNodeVariant  = 0x10;
} // namespace PowerV1

namespace PowerV3
{
    static const uint8_t kNodeAddress  = 2;
    static const uint16_t kNodeSupplier = 0xb007;
    static const uint16_t kNodeFunction = 0x0002;
    static const uint8_t kNodeVariant  = 0x01;
} // namespace PowerV3

namespace ECUBridge
{
    static const uint8_t kNodeAddress  = 18;
    static const uint16_t kNodeSupplier = 0xb007;
    static const uint16_t kNodeFunction = 0x0003;
    static const uint8_t kNodeVariant  = 0x10;
} // namespace ECUBridge

namespace Dashboard
{
    static const uint8_t kNodeAddress  = 19;
    static const uint16_t kNodeSupplier = 0xb007;
    static const uint16_t kNodeFunction = 0x0004;
    static const uint8_t kNodeVariant  = 0x01;
} // namespace Dashboard

namespace Tester
{
    static const uint8_t kNodeAddress  = 20;
    static const uint16_t kNodeSupplier = 0xb007;
    static const uint16_t kNodeFunction = 0x0005;
    static const uint8_t kNodeVariant  = 0x01;
} // namespace Tester

static const uint8_t kNodeAddressMax = 32;
