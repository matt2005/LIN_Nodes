// AUTOMATICALLY GENERATED - DO NOT EDIT
#pragma once
#include "lin_types.h"

struct ParamInfo { uint16_t def; uint8_t encoding; }

// Frame: Relays
static const uint8_t kFrameIDRelays = 0x01;
typedef Signal<bool, 0, 1> signal_Ignition;
typedef Signal<bool, 1, 1> signal_Start;
typedef Signal<bool, 2, 1> signal_LightsUp;
typedef Signal<bool, 3, 1> signal_LightsDown;
typedef Signal<bool, 4, 1> signal_HeadLights;
typedef Signal<bool, 5, 1> signal_LowBeam;
typedef Signal<bool, 6, 1> signal_HighBeam;
typedef Signal<bool, 7, 1> signal_FogLights;
typedef Signal<bool, 8, 1> signal_Markers;
typedef Signal<bool, 9, 1> signal_LeftTurn;
typedef Signal<bool, 10, 1> signal_LeftTurnMark;
typedef Signal<bool, 11, 1> signal_RightTurn;
typedef Signal<bool, 12, 1> signal_RightTurnMark;
typedef Signal<bool, 13, 1> signal_Brake;
typedef Signal<bool, 14, 1> signal_Reverse;
typedef Signal<bool, 15, 1> signal_InteriorLight;
typedef Signal<bool, 16, 1> signal_CabinFan1;
typedef Signal<bool, 17, 1> signal_CabinFan2;
typedef Signal<bool, 18, 1> signal_CabinFan3;
typedef Signal<bool, 19, 1> signal_CabinFan4;
typedef Signal<bool, 20, 1> signal_WiperLow;
typedef Signal<bool, 21, 1> signal_WiperHigh;
typedef Signal<bool, 22, 1> signal_RearDefrost;

// Frame: MasterRequest
static const uint8_t kFrameIDMasterRequest = 0x3c;
typedef Signal<uint8_t, 0, 8> signal_nad;
typedef Signal<uint8_t, 8, 4> signal_length;
typedef Signal<uint8_t, 12, 4> signal_pci;
typedef Signal<uint8_t, 16, 8> signal_sid;
typedef Signal<uint8_t, 24, 8> signal_d1;
typedef Signal<uint8_t, 32, 8> signal_d2;
typedef Signal<uint8_t, 40, 8> signal_d3;
typedef Signal<uint8_t, 48, 8> signal_d4;
typedef Signal<uint8_t, 56, 8> signal_d5;

// Frame: DataByID
typedef Signal<uint8_t, 0, 8> signal_nad;
typedef Signal<uint8_t, 8, 4> signal_length;
typedef Signal<uint8_t, 12, 4> signal_pci;
typedef Signal<uint8_t, 16, 8> signal_sid;
typedef Signal<uint16_t, 24, 16> signal_index;
typedef Signal<uint16_t, 40, 16> signal_value;
typedef Signal<uint8_t, 56, 8> signal_d5;

// Frame: SlaveResponse
static const uint8_t kFrameIDSlaveResponse = 0x3c;
typedef Signal<uint8_t, 0, 8> signal_nad;
typedef Signal<uint8_t, 8, 4> signal_length;
typedef Signal<uint8_t, 12, 4> signal_pci;
typedef Signal<uint8_t, 16, 8> signal_sid;
typedef Signal<uint8_t, 24, 8> signal_d1;
typedef Signal<uint8_t, 32, 8> signal_d2;
typedef Signal<uint8_t, 40, 8> signal_d3;
typedef Signal<uint8_t, 48, 8> signal_d4;
typedef Signal<uint8_t, 56, 8> signal_d5;

// Frame: ProxyRequest
static const uint8_t kFrameIDProxyRequest = 0x3d;

// Frame: ECUData
static const uint8_t kFrameIDECUData = 0x02;
typedef Signal<uint8_t, 0, 8> signal_RPM;
typedef Signal<uint8_t, 8, 8> signal_oilPressure;
typedef Signal<uint8_t, 16, 8> signal_oilTemperature;
typedef Signal<uint8_t, 24, 8> signal_coolantTemperature;
typedef Signal<uint8_t, 32, 8> signal_fuelPressure;
typedef Signal<uint8_t, 40, 8> signal_voltage;
typedef Signal<uint8_t, 48, 8> signal_AFR;
typedef Signal<uint8_t, 56, 8> signal_roadSpeed;

// Encodings
namespace Encoding
{
extern bool validate_pwm_duty_cycle(uint16_t value) const;
extern bool validate_output_assignment(uint16_t value) const;
extern bool validate_sec(uint16_t value) const;
extern bool validate_PSI(uint16_t value) const;
extern bool validate_MPH(uint16_t value) const;
extern bool validate_F(uint16_t value) const;
extern bool validate_V(uint16_t value) const;
extern bool validate_up_to_10(uint16_t value) const;
extern bool validate_input(uint16_t value) const;
extern bool validate_msec(uint16_t value) const;
extern bool validate_lambda(uint16_t value) const;
extern bool validate_output_type(uint16_t value) const;
extern bool validate_service_id(uint16_t value) const;
extern bool validate_RPM(uint16_t value) const;
} // namepsace Encoding

// Node: Dashboard
namespace Dashboard
{
static const uint8_t kNodeAddress  = 19;
static const uint8_t kNodeSupplier = 0xb007;
static const uint8_t kNodeFunction = 0x0004;
static const uint8_t kNodeVariant  = 0x01;
static const uint8_t kNumEncodings = 0;
} // namespace Dashboard

// Node: PowerV1
namespace PowerV1
{
static const uint8_t kNodeAddress  = 2;
static const uint8_t kNodeSupplier = 0xb007;
static const uint8_t kNodeFunction = 0x0001;
static const uint8_t kNodeVariant  = 0x10;
static const uint8_t kNumEncodings = 4;
extern const PROGMEM encoding_table[kNumEncodings];
extern const PROGMEM Parameter::ParamInfo page4_info;
} // namespace PowerV1

// Node: Master
namespace Master
{
static const uint8_t kNodeAddress  = 1;
static const uint8_t kNodeSupplier = 0xb007;
static const uint8_t kNodeFunction = 0x0000;
static const uint8_t kNodeVariant  = 0x01;
static const uint8_t kNumEncodings = 29;
extern const PROGMEM encoding_table[kNumEncodings];
extern const PROGMEM Parameter::ParamInfo page4_info;
} // namespace Master

// Node: Generic
namespace Generic
{
static const uint8_t kNodeAddress  = 1;
static const uint8_t kNodeSupplier = 0xb007;
static const uint8_t kNodeFunction = 0x0000;
static const uint8_t kNodeVariant  = 0x00;
static const uint8_t kNumEncodings = 0;
} // namespace Generic

// Node: ECUBridge
namespace ECUBridge
{
static const uint8_t kNodeAddress  = 18;
static const uint8_t kNodeSupplier = 0xb007;
static const uint8_t kNodeFunction = 0x0003;
static const uint8_t kNodeVariant  = 0x10;
static const uint8_t kNumEncodings = 0;
} // namespace ECUBridge

// Node: PowerV3
namespace PowerV3
{
static const uint8_t kNodeAddress  = 2;
static const uint8_t kNodeSupplier = 0xb007;
static const uint8_t kNodeFunction = 0x0002;
static const uint8_t kNodeVariant  = 0x01;
static const uint8_t kNumEncodings = 45;
extern const PROGMEM encoding_table[kNumEncodings];
extern const PROGMEM Parameter::ParamInfo page4_info;
} // namespace PowerV3

// Node: Tester
namespace Tester
{
static const uint8_t kNodeAddress  = 20;
static const uint8_t kNodeSupplier = 0xb007;
static const uint8_t kNodeFunction = 0x0005;
static const uint8_t kNodeVariant  = 0x01;
static const uint8_t kNumEncodings = 0;
} // namespace Tester
