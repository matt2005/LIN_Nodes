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
static const uint8_t kEncoding_service_id = 4;
static const uint8_t kEncoding_output_assignment = 5;
static const uint8_t kEncoding_output_type = 6;
static const uint8_t kEncoding_pwm_duty_cycle = 7;
static const uint8_t kEncoding_RPM = 8;
static const uint8_t kEncoding_PSI = 9;
static const uint8_t kEncoding_F = 10;
static const uint8_t kEncoding_V = 11;
static const uint8_t kEncoding_lambda = 12;
static const uint8_t kEncoding_MPH = 13;
static const uint8_t kEncoding_none = 0xff;

typedef Signal<uint8_t, 0, 8, kEncoding_none> signal_nad;
typedef Signal<uint8_t, 8, 4, kEncoding_none> signal_length;
typedef Signal<uint8_t, 12, 4, kEncoding_none> signal_pci;
typedef Signal<uint8_t, 16, 8, kEncoding_service_id> signal_sid;
typedef Signal<uint8_t, 24, 8, kEncoding_none> signal_d1;
typedef Signal<uint8_t, 32, 8, kEncoding_none> signal_d2;
typedef Signal<uint8_t, 40, 8, kEncoding_none> signal_d3;
typedef Signal<uint8_t, 48, 8, kEncoding_none> signal_d4;
typedef Signal<uint8_t, 56, 8, kEncoding_none> signal_d5;
typedef Signal<uint16_t, 24, 16, kEncoding_none> signal_index;
typedef Signal<uint16_t, 40, 16, kEncoding_none> signal_value;
typedef Signal<bool, 0, 1, kEncoding_none> signal_Ignition;
typedef Signal<bool, 1, 1, kEncoding_none> signal_Start;
typedef Signal<bool, 2, 1, kEncoding_none> signal_LightsUp;
typedef Signal<bool, 3, 1, kEncoding_none> signal_LightsDown;
typedef Signal<bool, 4, 1, kEncoding_none> signal_HeadLights;
typedef Signal<bool, 5, 1, kEncoding_none> signal_LowBeam;
typedef Signal<bool, 6, 1, kEncoding_none> signal_HighBeam;
typedef Signal<bool, 7, 1, kEncoding_none> signal_FogLights;
typedef Signal<bool, 8, 1, kEncoding_none> signal_Markers;
typedef Signal<bool, 9, 1, kEncoding_none> signal_LeftTurn;
typedef Signal<bool, 10, 1, kEncoding_none> signal_LeftTurnMark;
typedef Signal<bool, 11, 1, kEncoding_none> signal_RightTurn;
typedef Signal<bool, 12, 1, kEncoding_none> signal_RightTurnMark;
typedef Signal<bool, 13, 1, kEncoding_none> signal_Brake;
typedef Signal<bool, 14, 1, kEncoding_none> signal_Reverse;
typedef Signal<bool, 15, 1, kEncoding_none> signal_InteriorLight;
typedef Signal<bool, 16, 1, kEncoding_none> signal_CabinFan1;
typedef Signal<bool, 17, 1, kEncoding_none> signal_CabinFan2;
typedef Signal<bool, 18, 1, kEncoding_none> signal_CabinFan3;
typedef Signal<bool, 19, 1, kEncoding_none> signal_CabinFan4;
typedef Signal<bool, 20, 1, kEncoding_none> signal_WiperLow;
typedef Signal<bool, 21, 1, kEncoding_none> signal_WiperHigh;
typedef Signal<bool, 22, 1, kEncoding_none> signal_RearDefrost;
typedef Signal<uint8_t, 0, 8, kEncoding_RPM> signal_RPM;
typedef Signal<uint8_t, 8, 8, kEncoding_PSI> signal_oilPressure;
typedef Signal<uint8_t, 16, 8, kEncoding_F> signal_oilTemperature;
typedef Signal<uint8_t, 24, 8, kEncoding_F> signal_coolantTemperature;
typedef Signal<uint8_t, 32, 8, kEncoding_PSI> signal_fuelPressure;
typedef Signal<uint8_t, 40, 8, kEncoding_V> signal_voltage;
typedef Signal<uint8_t, 48, 8, kEncoding_lambda> signal_AFR;
typedef Signal<uint8_t, 56, 8, kEncoding_MPH> signal_roadSpeed;
static const uint8_t kFrameIDSlaveResponse = 0x3c;
static const uint8_t kFrameIDRelays = 0x01;
static const uint8_t kFrameIDMasterRequest = 0x3c;
static const uint8_t kFrameIDECUData = 0x02;
static const uint8_t kFrameIDProxyRequest = 0x3d;

namespace Generic
{
extern void param_default(const Parameter &param);
extern const PROGMEM char *param_name(const Parameter &param);

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

// parameter factory for Generic
constexpr Parameter
parameter(Parameter::Address address)
{
    return Parameter((
        (address == kParamProtocolVersion) ? address :
        (address == kParamBoardFunction) ? address :
        (address == kParamBootloaderMode) ? address :
        (address == kParamFirmwareVersion) ? address :
        (address == kParamFirmwarePageSize) ? address :
        (address == kParamWatchdogResets) ? address :
        (address == kParamSupplyVoltage) ? address :
        (address == kParamTemperature) ? address :
        (address == kParamLine) ? address :
        (address == kParamChecksum) ? address :
        (address == kParamParity) ? address :
        (address == kParamFraming) ? address :
        (address == kParamSynch) ? address :
        (address == kParamProtocol) ? address :
        Parameter::noAddress), (
        kEncoding_none),
        param_default);
}

// explicit parameter subclasses for Generic
class ParamProtocolVersion : public Parameter
{
public:
    constexpr ParamProtocolVersion() : Parameter(kParamProtocolVersion, kEncoding_none, param_default) {}
};
class ParamBoardFunction : public Parameter
{
public:
    constexpr ParamBoardFunction() : Parameter(kParamBoardFunction, kEncoding_none, param_default) {}
};
class ParamBootloaderMode : public Parameter
{
public:
    constexpr ParamBootloaderMode() : Parameter(kParamBootloaderMode, kEncoding_none, param_default) {}
};
class ParamFirmwareVersion : public Parameter
{
public:
    constexpr ParamFirmwareVersion() : Parameter(kParamFirmwareVersion, kEncoding_none, param_default) {}
};
class ParamFirmwarePageSize : public Parameter
{
public:
    constexpr ParamFirmwarePageSize() : Parameter(kParamFirmwarePageSize, kEncoding_none, param_default) {}
};
class ParamWatchdogResets : public Parameter
{
public:
    constexpr ParamWatchdogResets() : Parameter(kParamWatchdogResets, kEncoding_none, param_default) {}
};
class ParamSupplyVoltage : public Parameter
{
public:
    constexpr ParamSupplyVoltage() : Parameter(kParamSupplyVoltage, kEncoding_none, param_default) {}
};
class ParamTemperature : public Parameter
{
public:
    constexpr ParamTemperature() : Parameter(kParamTemperature, kEncoding_none, param_default) {}
};
class ParamLine : public Parameter
{
public:
    constexpr ParamLine() : Parameter(kParamLine, kEncoding_none, param_default) {}
};
class ParamChecksum : public Parameter
{
public:
    constexpr ParamChecksum() : Parameter(kParamChecksum, kEncoding_none, param_default) {}
};
class ParamParity : public Parameter
{
public:
    constexpr ParamParity() : Parameter(kParamParity, kEncoding_none, param_default) {}
};
class ParamFraming : public Parameter
{
public:
    constexpr ParamFraming() : Parameter(kParamFraming, kEncoding_none, param_default) {}
};
class ParamSynch : public Parameter
{
public:
    constexpr ParamSynch() : Parameter(kParamSynch, kEncoding_none, param_default) {}
};
class ParamProtocol : public Parameter
{
public:
    constexpr ParamProtocol() : Parameter(kParamProtocol, kEncoding_none, param_default) {}
};

} // namespace Generic

namespace Master
{
extern void param_default(const Parameter &param);
extern const PROGMEM char *param_name(const Parameter &param);

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
static const uint16_t kParamBrakeBlinkPeriod = 0x0420;
static const uint16_t kParamBrakeBlinkCount = 0x0421;
static const uint16_t kParamWiperInterval = 0x0422;

// parameter factory for Master
constexpr Parameter
parameter(Parameter::Address address)
{
    return Parameter((
        (address == kParamSP1Assign) ? address :
        (address == kParamSP2Assign) ? address :
        (address == kParamSP3Assign) ? address :
        (address == kParamSP4Assign) ? address :
        (address == kParamSP5Assign) ? address :
        (address == kParamSP6Assign) ? address :
        (address == kParamSP7Assign) ? address :
        (address == kParamSG0Assign) ? address :
        (address == kParamSG1Assign) ? address :
        (address == kParamSG2Assign) ? address :
        (address == kParamSG3Assign) ? address :
        (address == kParamSG4Assign) ? address :
        (address == kParamSG5Assign) ? address :
        (address == kParamSG6Assign) ? address :
        (address == kParamSG7Assign) ? address :
        (address == kParamSG8Assign) ? address :
        (address == kParamSG9Assign) ? address :
        (address == kParamSG10Assign) ? address :
        (address == kParamSG11Assign) ? address :
        (address == kParamSG12Assign) ? address :
        (address == kParamSG13Assign) ? address :
        (address == kParamTurnBlinkPeriod) ? address :
        (address == kParamPassingBlinkCount) ? address :
        (address == kParamPathLightTime) ? address :
        (address == kParamInteriorLightTime) ? address :
        (address == kParamWelcomeLightTime) ? address :
        (address == kParamBrakeBlinkPeriod) ? address :
        (address == kParamBrakeBlinkCount) ? address :
        (address == kParamWiperInterval) ? address :
        Parameter::noAddress), (
        (address == kParamSP1Assign) ? kEncoding_input :
        (address == kParamSP2Assign) ? kEncoding_input :
        (address == kParamSP3Assign) ? kEncoding_input :
        (address == kParamSP4Assign) ? kEncoding_input :
        (address == kParamSP5Assign) ? kEncoding_input :
        (address == kParamSP6Assign) ? kEncoding_input :
        (address == kParamSP7Assign) ? kEncoding_input :
        (address == kParamSG0Assign) ? kEncoding_input :
        (address == kParamSG1Assign) ? kEncoding_input :
        (address == kParamSG2Assign) ? kEncoding_input :
        (address == kParamSG3Assign) ? kEncoding_input :
        (address == kParamSG4Assign) ? kEncoding_input :
        (address == kParamSG5Assign) ? kEncoding_input :
        (address == kParamSG6Assign) ? kEncoding_input :
        (address == kParamSG7Assign) ? kEncoding_input :
        (address == kParamSG8Assign) ? kEncoding_input :
        (address == kParamSG9Assign) ? kEncoding_input :
        (address == kParamSG10Assign) ? kEncoding_input :
        (address == kParamSG11Assign) ? kEncoding_input :
        (address == kParamSG12Assign) ? kEncoding_input :
        (address == kParamSG13Assign) ? kEncoding_input :
        (address == kParamTurnBlinkPeriod) ? kEncoding_msec :
        (address == kParamPassingBlinkCount) ? kEncoding_up_to_10 :
        (address == kParamPathLightTime) ? kEncoding_sec :
        (address == kParamInteriorLightTime) ? kEncoding_sec :
        (address == kParamWelcomeLightTime) ? kEncoding_sec :
        (address == kParamBrakeBlinkPeriod) ? kEncoding_msec :
        (address == kParamBrakeBlinkCount) ? kEncoding_up_to_10 :
        (address == kParamWiperInterval) ? kEncoding_msec :
        kEncoding_none),
        param_default);
}

// explicit parameter subclasses for Master
class ParamSP1Assign : public Parameter
{
public:
    constexpr ParamSP1Assign() : Parameter(kParamSP1Assign, kEncoding_input, param_default) {}
};
class ParamSP2Assign : public Parameter
{
public:
    constexpr ParamSP2Assign() : Parameter(kParamSP2Assign, kEncoding_input, param_default) {}
};
class ParamSP3Assign : public Parameter
{
public:
    constexpr ParamSP3Assign() : Parameter(kParamSP3Assign, kEncoding_input, param_default) {}
};
class ParamSP4Assign : public Parameter
{
public:
    constexpr ParamSP4Assign() : Parameter(kParamSP4Assign, kEncoding_input, param_default) {}
};
class ParamSP5Assign : public Parameter
{
public:
    constexpr ParamSP5Assign() : Parameter(kParamSP5Assign, kEncoding_input, param_default) {}
};
class ParamSP6Assign : public Parameter
{
public:
    constexpr ParamSP6Assign() : Parameter(kParamSP6Assign, kEncoding_input, param_default) {}
};
class ParamSP7Assign : public Parameter
{
public:
    constexpr ParamSP7Assign() : Parameter(kParamSP7Assign, kEncoding_input, param_default) {}
};
class ParamSG0Assign : public Parameter
{
public:
    constexpr ParamSG0Assign() : Parameter(kParamSG0Assign, kEncoding_input, param_default) {}
};
class ParamSG1Assign : public Parameter
{
public:
    constexpr ParamSG1Assign() : Parameter(kParamSG1Assign, kEncoding_input, param_default) {}
};
class ParamSG2Assign : public Parameter
{
public:
    constexpr ParamSG2Assign() : Parameter(kParamSG2Assign, kEncoding_input, param_default) {}
};
class ParamSG3Assign : public Parameter
{
public:
    constexpr ParamSG3Assign() : Parameter(kParamSG3Assign, kEncoding_input, param_default) {}
};
class ParamSG4Assign : public Parameter
{
public:
    constexpr ParamSG4Assign() : Parameter(kParamSG4Assign, kEncoding_input, param_default) {}
};
class ParamSG5Assign : public Parameter
{
public:
    constexpr ParamSG5Assign() : Parameter(kParamSG5Assign, kEncoding_input, param_default) {}
};
class ParamSG6Assign : public Parameter
{
public:
    constexpr ParamSG6Assign() : Parameter(kParamSG6Assign, kEncoding_input, param_default) {}
};
class ParamSG7Assign : public Parameter
{
public:
    constexpr ParamSG7Assign() : Parameter(kParamSG7Assign, kEncoding_input, param_default) {}
};
class ParamSG8Assign : public Parameter
{
public:
    constexpr ParamSG8Assign() : Parameter(kParamSG8Assign, kEncoding_input, param_default) {}
};
class ParamSG9Assign : public Parameter
{
public:
    constexpr ParamSG9Assign() : Parameter(kParamSG9Assign, kEncoding_input, param_default) {}
};
class ParamSG10Assign : public Parameter
{
public:
    constexpr ParamSG10Assign() : Parameter(kParamSG10Assign, kEncoding_input, param_default) {}
};
class ParamSG11Assign : public Parameter
{
public:
    constexpr ParamSG11Assign() : Parameter(kParamSG11Assign, kEncoding_input, param_default) {}
};
class ParamSG12Assign : public Parameter
{
public:
    constexpr ParamSG12Assign() : Parameter(kParamSG12Assign, kEncoding_input, param_default) {}
};
class ParamSG13Assign : public Parameter
{
public:
    constexpr ParamSG13Assign() : Parameter(kParamSG13Assign, kEncoding_input, param_default) {}
};
class ParamTurnBlinkPeriod : public Parameter
{
public:
    constexpr ParamTurnBlinkPeriod() : Parameter(kParamTurnBlinkPeriod, kEncoding_msec, param_default) {}
};
class ParamPassingBlinkCount : public Parameter
{
public:
    constexpr ParamPassingBlinkCount() : Parameter(kParamPassingBlinkCount, kEncoding_up_to_10, param_default) {}
};
class ParamPathLightTime : public Parameter
{
public:
    constexpr ParamPathLightTime() : Parameter(kParamPathLightTime, kEncoding_sec, param_default) {}
};
class ParamInteriorLightTime : public Parameter
{
public:
    constexpr ParamInteriorLightTime() : Parameter(kParamInteriorLightTime, kEncoding_sec, param_default) {}
};
class ParamWelcomeLightTime : public Parameter
{
public:
    constexpr ParamWelcomeLightTime() : Parameter(kParamWelcomeLightTime, kEncoding_sec, param_default) {}
};
class ParamBrakeBlinkPeriod : public Parameter
{
public:
    constexpr ParamBrakeBlinkPeriod() : Parameter(kParamBrakeBlinkPeriod, kEncoding_msec, param_default) {}
};
class ParamBrakeBlinkCount : public Parameter
{
public:
    constexpr ParamBrakeBlinkCount() : Parameter(kParamBrakeBlinkCount, kEncoding_up_to_10, param_default) {}
};
class ParamWiperInterval : public Parameter
{
public:
    constexpr ParamWiperInterval() : Parameter(kParamWiperInterval, kEncoding_msec, param_default) {}
};

} // namespace Master

namespace PowerV1
{
extern void param_default(const Parameter &param);
extern const PROGMEM char *param_name(const Parameter &param);

static const uint16_t kParamRelay1Assign = 0x0400;
static const uint16_t kParamRelay2Assign = 0x0401;
static const uint16_t kParamRelay3Assign = 0x0402;
static const uint16_t kParamRelay4Assign = 0x0403;

// parameter factory for PowerV1
constexpr Parameter
parameter(Parameter::Address address)
{
    return Parameter((
        (address == kParamRelay1Assign) ? address :
        (address == kParamRelay2Assign) ? address :
        (address == kParamRelay3Assign) ? address :
        (address == kParamRelay4Assign) ? address :
        Parameter::noAddress), (
        (address == kParamRelay1Assign) ? kEncoding_output_assignment :
        (address == kParamRelay2Assign) ? kEncoding_output_assignment :
        (address == kParamRelay3Assign) ? kEncoding_output_assignment :
        (address == kParamRelay4Assign) ? kEncoding_output_assignment :
        kEncoding_none),
        param_default);
}

// explicit parameter subclasses for PowerV1
class ParamRelay1Assign : public Parameter
{
public:
    constexpr ParamRelay1Assign() : Parameter(kParamRelay1Assign, kEncoding_output_assignment, param_default) {}
};
class ParamRelay2Assign : public Parameter
{
public:
    constexpr ParamRelay2Assign() : Parameter(kParamRelay2Assign, kEncoding_output_assignment, param_default) {}
};
class ParamRelay3Assign : public Parameter
{
public:
    constexpr ParamRelay3Assign() : Parameter(kParamRelay3Assign, kEncoding_output_assignment, param_default) {}
};
class ParamRelay4Assign : public Parameter
{
public:
    constexpr ParamRelay4Assign() : Parameter(kParamRelay4Assign, kEncoding_output_assignment, param_default) {}
};

} // namespace PowerV1

namespace PowerV3
{
extern void param_default(const Parameter &param);
extern const PROGMEM char *param_name(const Parameter &param);

static const uint16_t kParamCH1Type = 0x0400;
static const uint16_t kParamCH2Type = 0x0401;
static const uint16_t kParamCH3Type = 0x0402;
static const uint16_t kParamCH4Type = 0x0403;
static const uint16_t kParamCH5Type = 0x0404;
static const uint16_t kParamCH1Assign1 = 0x0410;
static const uint16_t kParamCH1PWM1 = 0x0411;
static const uint16_t kParamCH1Assign2 = 0x0412;
static const uint16_t kParamCH1PWM2 = 0x0413;
static const uint16_t kParamCH1Assign3 = 0x0414;
static const uint16_t kParamCH1PWM3 = 0x0415;
static const uint16_t kParamCH1Assign4 = 0x0416;
static const uint16_t kParamCH1PWM4 = 0x0417;
static const uint16_t kParamCH2Assign1 = 0x0420;
static const uint16_t kParamCH2PWM1 = 0x0421;
static const uint16_t kParamCH2Assign2 = 0x0422;
static const uint16_t kParamCH2PWM2 = 0x0423;
static const uint16_t kParamCH2Assign3 = 0x0424;
static const uint16_t kParamCH2PWM3 = 0x0425;
static const uint16_t kParamCH2Assign4 = 0x0426;
static const uint16_t kParamCH2PWM4 = 0x0427;
static const uint16_t kParamCH3Assign1 = 0x0430;
static const uint16_t kParamCH3PWM1 = 0x0431;
static const uint16_t kParamCH3Assign2 = 0x0432;
static const uint16_t kParamCH3PWM2 = 0x0433;
static const uint16_t kParamCH3Assign3 = 0x0434;
static const uint16_t kParamCH3PWM3 = 0x0435;
static const uint16_t kParamCH3Assign4 = 0x0436;
static const uint16_t kParamCH3PWM4 = 0x0437;
static const uint16_t kParamCH4Assign1 = 0x0440;
static const uint16_t kParamCH4PWM1 = 0x0441;
static const uint16_t kParamCH4Assign2 = 0x0442;
static const uint16_t kParamCH4PWM2 = 0x0443;
static const uint16_t kParamCH4Assign3 = 0x0444;
static const uint16_t kParamCH4PWM3 = 0x0445;
static const uint16_t kParamCH4Assign4 = 0x0446;
static const uint16_t kParamCH4PWM4 = 0x0447;
static const uint16_t kParamCH5Assign1 = 0x0450;
static const uint16_t kParamCH5PWM1 = 0x0451;
static const uint16_t kParamCH5Assign2 = 0x0452;
static const uint16_t kParamCH5PWM2 = 0x0453;
static const uint16_t kParamCH5Assign3 = 0x0454;
static const uint16_t kParamCH5PWM3 = 0x0455;
static const uint16_t kParamCH5Assign4 = 0x0456;
static const uint16_t kParamCH5PWM4 = 0x0457;

// parameter factory for PowerV3
constexpr Parameter
parameter(Parameter::Address address)
{
    return Parameter((
        (address == kParamCH1Type) ? address :
        (address == kParamCH2Type) ? address :
        (address == kParamCH3Type) ? address :
        (address == kParamCH4Type) ? address :
        (address == kParamCH5Type) ? address :
        (address == kParamCH1Assign1) ? address :
        (address == kParamCH1PWM1) ? address :
        (address == kParamCH1Assign2) ? address :
        (address == kParamCH1PWM2) ? address :
        (address == kParamCH1Assign3) ? address :
        (address == kParamCH1PWM3) ? address :
        (address == kParamCH1Assign4) ? address :
        (address == kParamCH1PWM4) ? address :
        (address == kParamCH2Assign1) ? address :
        (address == kParamCH2PWM1) ? address :
        (address == kParamCH2Assign2) ? address :
        (address == kParamCH2PWM2) ? address :
        (address == kParamCH2Assign3) ? address :
        (address == kParamCH2PWM3) ? address :
        (address == kParamCH2Assign4) ? address :
        (address == kParamCH2PWM4) ? address :
        (address == kParamCH3Assign1) ? address :
        (address == kParamCH3PWM1) ? address :
        (address == kParamCH3Assign2) ? address :
        (address == kParamCH3PWM2) ? address :
        (address == kParamCH3Assign3) ? address :
        (address == kParamCH3PWM3) ? address :
        (address == kParamCH3Assign4) ? address :
        (address == kParamCH3PWM4) ? address :
        (address == kParamCH4Assign1) ? address :
        (address == kParamCH4PWM1) ? address :
        (address == kParamCH4Assign2) ? address :
        (address == kParamCH4PWM2) ? address :
        (address == kParamCH4Assign3) ? address :
        (address == kParamCH4PWM3) ? address :
        (address == kParamCH4Assign4) ? address :
        (address == kParamCH4PWM4) ? address :
        (address == kParamCH5Assign1) ? address :
        (address == kParamCH5PWM1) ? address :
        (address == kParamCH5Assign2) ? address :
        (address == kParamCH5PWM2) ? address :
        (address == kParamCH5Assign3) ? address :
        (address == kParamCH5PWM3) ? address :
        (address == kParamCH5Assign4) ? address :
        (address == kParamCH5PWM4) ? address :
        Parameter::noAddress), (
        (address == kParamCH1Type) ? kEncoding_output_type :
        (address == kParamCH2Type) ? kEncoding_output_type :
        (address == kParamCH3Type) ? kEncoding_output_type :
        (address == kParamCH4Type) ? kEncoding_output_type :
        (address == kParamCH5Type) ? kEncoding_output_type :
        (address == kParamCH1Assign1) ? kEncoding_output_assignment :
        (address == kParamCH1PWM1) ? kEncoding_pwm_duty_cycle :
        (address == kParamCH1Assign2) ? kEncoding_output_assignment :
        (address == kParamCH1PWM2) ? kEncoding_pwm_duty_cycle :
        (address == kParamCH1Assign3) ? kEncoding_output_assignment :
        (address == kParamCH1PWM3) ? kEncoding_pwm_duty_cycle :
        (address == kParamCH1Assign4) ? kEncoding_output_assignment :
        (address == kParamCH1PWM4) ? kEncoding_pwm_duty_cycle :
        (address == kParamCH2Assign1) ? kEncoding_output_assignment :
        (address == kParamCH2PWM1) ? kEncoding_pwm_duty_cycle :
        (address == kParamCH2Assign2) ? kEncoding_output_assignment :
        (address == kParamCH2PWM2) ? kEncoding_pwm_duty_cycle :
        (address == kParamCH2Assign3) ? kEncoding_output_assignment :
        (address == kParamCH2PWM3) ? kEncoding_pwm_duty_cycle :
        (address == kParamCH2Assign4) ? kEncoding_output_assignment :
        (address == kParamCH2PWM4) ? kEncoding_pwm_duty_cycle :
        (address == kParamCH3Assign1) ? kEncoding_output_assignment :
        (address == kParamCH3PWM1) ? kEncoding_pwm_duty_cycle :
        (address == kParamCH3Assign2) ? kEncoding_output_assignment :
        (address == kParamCH3PWM2) ? kEncoding_pwm_duty_cycle :
        (address == kParamCH3Assign3) ? kEncoding_output_assignment :
        (address == kParamCH3PWM3) ? kEncoding_pwm_duty_cycle :
        (address == kParamCH3Assign4) ? kEncoding_output_assignment :
        (address == kParamCH3PWM4) ? kEncoding_pwm_duty_cycle :
        (address == kParamCH4Assign1) ? kEncoding_output_assignment :
        (address == kParamCH4PWM1) ? kEncoding_pwm_duty_cycle :
        (address == kParamCH4Assign2) ? kEncoding_output_assignment :
        (address == kParamCH4PWM2) ? kEncoding_pwm_duty_cycle :
        (address == kParamCH4Assign3) ? kEncoding_output_assignment :
        (address == kParamCH4PWM3) ? kEncoding_pwm_duty_cycle :
        (address == kParamCH4Assign4) ? kEncoding_output_assignment :
        (address == kParamCH4PWM4) ? kEncoding_pwm_duty_cycle :
        (address == kParamCH5Assign1) ? kEncoding_output_assignment :
        (address == kParamCH5PWM1) ? kEncoding_pwm_duty_cycle :
        (address == kParamCH5Assign2) ? kEncoding_output_assignment :
        (address == kParamCH5PWM2) ? kEncoding_pwm_duty_cycle :
        (address == kParamCH5Assign3) ? kEncoding_output_assignment :
        (address == kParamCH5PWM3) ? kEncoding_pwm_duty_cycle :
        (address == kParamCH5Assign4) ? kEncoding_output_assignment :
        (address == kParamCH5PWM4) ? kEncoding_pwm_duty_cycle :
        kEncoding_none),
        param_default);
}

// explicit parameter subclasses for PowerV3
class ParamCH1Type : public Parameter
{
public:
    constexpr ParamCH1Type() : Parameter(kParamCH1Type, kEncoding_output_type, param_default) {}
};
class ParamCH2Type : public Parameter
{
public:
    constexpr ParamCH2Type() : Parameter(kParamCH2Type, kEncoding_output_type, param_default) {}
};
class ParamCH3Type : public Parameter
{
public:
    constexpr ParamCH3Type() : Parameter(kParamCH3Type, kEncoding_output_type, param_default) {}
};
class ParamCH4Type : public Parameter
{
public:
    constexpr ParamCH4Type() : Parameter(kParamCH4Type, kEncoding_output_type, param_default) {}
};
class ParamCH5Type : public Parameter
{
public:
    constexpr ParamCH5Type() : Parameter(kParamCH5Type, kEncoding_output_type, param_default) {}
};
class ParamCH1Assign1 : public Parameter
{
public:
    constexpr ParamCH1Assign1() : Parameter(kParamCH1Assign1, kEncoding_output_assignment, param_default) {}
};
class ParamCH1PWM1 : public Parameter
{
public:
    constexpr ParamCH1PWM1() : Parameter(kParamCH1PWM1, kEncoding_pwm_duty_cycle, param_default) {}
};
class ParamCH1Assign2 : public Parameter
{
public:
    constexpr ParamCH1Assign2() : Parameter(kParamCH1Assign2, kEncoding_output_assignment, param_default) {}
};
class ParamCH1PWM2 : public Parameter
{
public:
    constexpr ParamCH1PWM2() : Parameter(kParamCH1PWM2, kEncoding_pwm_duty_cycle, param_default) {}
};
class ParamCH1Assign3 : public Parameter
{
public:
    constexpr ParamCH1Assign3() : Parameter(kParamCH1Assign3, kEncoding_output_assignment, param_default) {}
};
class ParamCH1PWM3 : public Parameter
{
public:
    constexpr ParamCH1PWM3() : Parameter(kParamCH1PWM3, kEncoding_pwm_duty_cycle, param_default) {}
};
class ParamCH1Assign4 : public Parameter
{
public:
    constexpr ParamCH1Assign4() : Parameter(kParamCH1Assign4, kEncoding_output_assignment, param_default) {}
};
class ParamCH1PWM4 : public Parameter
{
public:
    constexpr ParamCH1PWM4() : Parameter(kParamCH1PWM4, kEncoding_pwm_duty_cycle, param_default) {}
};
class ParamCH2Assign1 : public Parameter
{
public:
    constexpr ParamCH2Assign1() : Parameter(kParamCH2Assign1, kEncoding_output_assignment, param_default) {}
};
class ParamCH2PWM1 : public Parameter
{
public:
    constexpr ParamCH2PWM1() : Parameter(kParamCH2PWM1, kEncoding_pwm_duty_cycle, param_default) {}
};
class ParamCH2Assign2 : public Parameter
{
public:
    constexpr ParamCH2Assign2() : Parameter(kParamCH2Assign2, kEncoding_output_assignment, param_default) {}
};
class ParamCH2PWM2 : public Parameter
{
public:
    constexpr ParamCH2PWM2() : Parameter(kParamCH2PWM2, kEncoding_pwm_duty_cycle, param_default) {}
};
class ParamCH2Assign3 : public Parameter
{
public:
    constexpr ParamCH2Assign3() : Parameter(kParamCH2Assign3, kEncoding_output_assignment, param_default) {}
};
class ParamCH2PWM3 : public Parameter
{
public:
    constexpr ParamCH2PWM3() : Parameter(kParamCH2PWM3, kEncoding_pwm_duty_cycle, param_default) {}
};
class ParamCH2Assign4 : public Parameter
{
public:
    constexpr ParamCH2Assign4() : Parameter(kParamCH2Assign4, kEncoding_output_assignment, param_default) {}
};
class ParamCH2PWM4 : public Parameter
{
public:
    constexpr ParamCH2PWM4() : Parameter(kParamCH2PWM4, kEncoding_pwm_duty_cycle, param_default) {}
};
class ParamCH3Assign1 : public Parameter
{
public:
    constexpr ParamCH3Assign1() : Parameter(kParamCH3Assign1, kEncoding_output_assignment, param_default) {}
};
class ParamCH3PWM1 : public Parameter
{
public:
    constexpr ParamCH3PWM1() : Parameter(kParamCH3PWM1, kEncoding_pwm_duty_cycle, param_default) {}
};
class ParamCH3Assign2 : public Parameter
{
public:
    constexpr ParamCH3Assign2() : Parameter(kParamCH3Assign2, kEncoding_output_assignment, param_default) {}
};
class ParamCH3PWM2 : public Parameter
{
public:
    constexpr ParamCH3PWM2() : Parameter(kParamCH3PWM2, kEncoding_pwm_duty_cycle, param_default) {}
};
class ParamCH3Assign3 : public Parameter
{
public:
    constexpr ParamCH3Assign3() : Parameter(kParamCH3Assign3, kEncoding_output_assignment, param_default) {}
};
class ParamCH3PWM3 : public Parameter
{
public:
    constexpr ParamCH3PWM3() : Parameter(kParamCH3PWM3, kEncoding_pwm_duty_cycle, param_default) {}
};
class ParamCH3Assign4 : public Parameter
{
public:
    constexpr ParamCH3Assign4() : Parameter(kParamCH3Assign4, kEncoding_output_assignment, param_default) {}
};
class ParamCH3PWM4 : public Parameter
{
public:
    constexpr ParamCH3PWM4() : Parameter(kParamCH3PWM4, kEncoding_pwm_duty_cycle, param_default) {}
};
class ParamCH4Assign1 : public Parameter
{
public:
    constexpr ParamCH4Assign1() : Parameter(kParamCH4Assign1, kEncoding_output_assignment, param_default) {}
};
class ParamCH4PWM1 : public Parameter
{
public:
    constexpr ParamCH4PWM1() : Parameter(kParamCH4PWM1, kEncoding_pwm_duty_cycle, param_default) {}
};
class ParamCH4Assign2 : public Parameter
{
public:
    constexpr ParamCH4Assign2() : Parameter(kParamCH4Assign2, kEncoding_output_assignment, param_default) {}
};
class ParamCH4PWM2 : public Parameter
{
public:
    constexpr ParamCH4PWM2() : Parameter(kParamCH4PWM2, kEncoding_pwm_duty_cycle, param_default) {}
};
class ParamCH4Assign3 : public Parameter
{
public:
    constexpr ParamCH4Assign3() : Parameter(kParamCH4Assign3, kEncoding_output_assignment, param_default) {}
};
class ParamCH4PWM3 : public Parameter
{
public:
    constexpr ParamCH4PWM3() : Parameter(kParamCH4PWM3, kEncoding_pwm_duty_cycle, param_default) {}
};
class ParamCH4Assign4 : public Parameter
{
public:
    constexpr ParamCH4Assign4() : Parameter(kParamCH4Assign4, kEncoding_output_assignment, param_default) {}
};
class ParamCH4PWM4 : public Parameter
{
public:
    constexpr ParamCH4PWM4() : Parameter(kParamCH4PWM4, kEncoding_pwm_duty_cycle, param_default) {}
};
class ParamCH5Assign1 : public Parameter
{
public:
    constexpr ParamCH5Assign1() : Parameter(kParamCH5Assign1, kEncoding_output_assignment, param_default) {}
};
class ParamCH5PWM1 : public Parameter
{
public:
    constexpr ParamCH5PWM1() : Parameter(kParamCH5PWM1, kEncoding_pwm_duty_cycle, param_default) {}
};
class ParamCH5Assign2 : public Parameter
{
public:
    constexpr ParamCH5Assign2() : Parameter(kParamCH5Assign2, kEncoding_output_assignment, param_default) {}
};
class ParamCH5PWM2 : public Parameter
{
public:
    constexpr ParamCH5PWM2() : Parameter(kParamCH5PWM2, kEncoding_pwm_duty_cycle, param_default) {}
};
class ParamCH5Assign3 : public Parameter
{
public:
    constexpr ParamCH5Assign3() : Parameter(kParamCH5Assign3, kEncoding_output_assignment, param_default) {}
};
class ParamCH5PWM3 : public Parameter
{
public:
    constexpr ParamCH5PWM3() : Parameter(kParamCH5PWM3, kEncoding_pwm_duty_cycle, param_default) {}
};
class ParamCH5Assign4 : public Parameter
{
public:
    constexpr ParamCH5Assign4() : Parameter(kParamCH5Assign4, kEncoding_output_assignment, param_default) {}
};
class ParamCH5PWM4 : public Parameter
{
public:
    constexpr ParamCH5PWM4() : Parameter(kParamCH5PWM4, kEncoding_pwm_duty_cycle, param_default) {}
};

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
