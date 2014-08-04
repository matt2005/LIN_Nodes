// AUTOMATICALLY GENERATED - DO NOT EDIT
#include "lin_defs.h"



bool
Encoding::invalid(uint8_t index, uint16_t value)
{
    switch(index) {
    case kEncoding_input:
        switch (value) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
            return false;
        }
        break;
    case kEncoding_msec:
        switch (value) {
        case 0:
        case 100 ... 65000:
            return false;
        }
        break;
    case kEncoding_up_to_10:
        switch (value) {
        case 0:
        case 1 ... 10:
            return false;
        }
        break;
    case kEncoding_sec:
        switch (value) {
        case 0:
        case 1 ... 600:
            return false;
        }
        break;
    case kEncoding_output_assignment:
        switch (value) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
            return false;
        }
        break;
    case kEncoding_output_type:
        switch (value) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            return false;
        }
        break;
    case kEncoding_pwm_duty_cycle:
        switch (value) {
        case 0 ... 100:
            return false;
        }
        break;
    default:
        return false;
    }
    return true;
}

static const PROGMEM char _encoding_names[] = 
    "input\0"
    "msec\0"
    "sec\0"
    "up_to_10\0"
    "service_id\0"
    "output_assignment\0"
    "output_type\0"
    "pwm_duty_cycle\0"
    "RPM\0"
    "PSI\0"
    "F\0"
    "V\0"
    "lambda\0"
    "MPH\0"
    "\0";

static const PROGMEM char _encoding_infos[] = 
    "Unassigned\0"
    "Ignition\0"
    "Start\0"
    "MarkerLight\0"
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
    "Disabled\0"
    "\0"
    "Disabled\0"
    "\0"
    "Disabled\0"
    "\0"
    "ReadDataByID\0"
    "WriteDataByID\0"
    "TesterPresent\0"
    "ReadByID\0"
    "DataDump\0"
    "ErrorResponse\0"
    "Unassigned\0"
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
    "5AGeneric\0"
    "10AGeneric\0"
    "LED\0"
    "HID\0"
    "LowPowerBulb\0"
    "HighPowerBulb\0"
    "Motor\0"
    "\0"
    "\0"
    "\0"
    "\0"
    "\0"
    "\0"
    "\0"
    "\0";

const PROGMEM char *
Encoding::name(uint8_t index)
{
    switch (index) {
    case kEncoding_input:
        return &_encoding_names[0];
    case kEncoding_msec:
        return &_encoding_names[6];
    case kEncoding_sec:
        return &_encoding_names[11];
    case kEncoding_up_to_10:
        return &_encoding_names[15];
    case kEncoding_service_id:
        return &_encoding_names[24];
    case kEncoding_output_assignment:
        return &_encoding_names[35];
    case kEncoding_output_type:
        return &_encoding_names[53];
    case kEncoding_pwm_duty_cycle:
        return &_encoding_names[65];
    case kEncoding_RPM:
        return &_encoding_names[80];
    case kEncoding_PSI:
        return &_encoding_names[84];
    case kEncoding_F:
        return &_encoding_names[88];
    case kEncoding_V:
        return &_encoding_names[90];
    case kEncoding_lambda:
        return &_encoding_names[92];
    case kEncoding_MPH:
        return &_encoding_names[99];
    }
    return nullptr;
}

const PROGMEM char *
Encoding::info(uint8_t index, uint16_t value)
{
    const char *info = nullptr;
    switch (index) {
    case kEncoding_input:
        switch (value) {
        case 0:
            info = &_encoding_infos[0];
            break;
        case 1:
            info = &_encoding_infos[11];
            break;
        case 2:
            info = &_encoding_infos[20];
            break;
        case 3:
            info = &_encoding_infos[26];
            break;
        case 4:
            info = &_encoding_infos[38];
            break;
        case 5:
            info = &_encoding_infos[49];
            break;
        case 6:
            info = &_encoding_infos[58];
            break;
        case 7:
            info = &_encoding_infos[73];
            break;
        case 8:
            info = &_encoding_infos[82];
            break;
        case 9:
            info = &_encoding_infos[91];
            break;
        case 10:
            info = &_encoding_infos[101];
            break;
        case 11:
            info = &_encoding_infos[107];
            break;
        case 12:
            info = &_encoding_infos[115];
            break;
        case 13:
            info = &_encoding_infos[120];
            break;
        case 14:
            info = &_encoding_infos[134];
            break;
        case 15:
            info = &_encoding_infos[141];
            break;
        case 16:
            info = &_encoding_infos[152];
            break;
        case 17:
            info = &_encoding_infos[161];
            break;
        case 18:
            info = &_encoding_infos[171];
            break;
        case 19:
            info = &_encoding_infos[181];
            break;
        case 20:
            info = &_encoding_infos[191];
            break;
        case 21:
            info = &_encoding_infos[200];
            break;
        case 22:
            info = &_encoding_infos[209];
            break;
        case 23:
            info = &_encoding_infos[219];
            break;
        }
        break;
    case kEncoding_msec:
        switch (value) {
        case 0:
            info = &_encoding_infos[231];
            break;
        case 100 ... 65000:
            info = &_encoding_infos[240];
            break;
        }
        break;
    case kEncoding_sec:
        switch (value) {
        case 0:
            info = &_encoding_infos[241];
            break;
        case 1 ... 600:
            info = &_encoding_infos[250];
            break;
        }
        break;
    case kEncoding_up_to_10:
        switch (value) {
        case 0:
            info = &_encoding_infos[251];
            break;
        case 1 ... 10:
            info = &_encoding_infos[260];
            break;
        }
        break;
    case kEncoding_service_id:
        switch (value) {
        case 0x22:
            info = &_encoding_infos[261];
            break;
        case 0x2e:
            info = &_encoding_infos[274];
            break;
        case 0x3e:
            info = &_encoding_infos[288];
            break;
        case 0xb2:
            info = &_encoding_infos[302];
            break;
        case 0xb4:
            info = &_encoding_infos[311];
            break;
        case 0x7f:
            info = &_encoding_infos[320];
            break;
        }
        break;
    case kEncoding_output_assignment:
        switch (value) {
        case 0:
            info = &_encoding_infos[334];
            break;
        case 1:
            info = &_encoding_infos[345];
            break;
        case 2:
            info = &_encoding_infos[354];
            break;
        case 3:
            info = &_encoding_infos[360];
            break;
        case 4:
            info = &_encoding_infos[369];
            break;
        case 5:
            info = &_encoding_infos[380];
            break;
        case 6:
            info = &_encoding_infos[391];
            break;
        case 7:
            info = &_encoding_infos[399];
            break;
        case 8:
            info = &_encoding_infos[408];
            break;
        case 9:
            info = &_encoding_infos[418];
            break;
        case 10:
            info = &_encoding_infos[426];
            break;
        case 11:
            info = &_encoding_infos[435];
            break;
        case 12:
            info = &_encoding_infos[450];
            break;
        case 13:
            info = &_encoding_infos[460];
            break;
        case 14:
            info = &_encoding_infos[476];
            break;
        case 15:
            info = &_encoding_infos[482];
            break;
        case 16:
            info = &_encoding_infos[490];
            break;
        case 17:
            info = &_encoding_infos[504];
            break;
        case 18:
            info = &_encoding_infos[514];
            break;
        case 19:
            info = &_encoding_infos[524];
            break;
        case 20:
            info = &_encoding_infos[534];
            break;
        case 21:
            info = &_encoding_infos[544];
            break;
        case 22:
            info = &_encoding_infos[553];
            break;
        case 23:
            info = &_encoding_infos[563];
            break;
        }
        break;
    case kEncoding_output_type:
        switch (value) {
        case 0:
            info = &_encoding_infos[575];
            break;
        case 1:
            info = &_encoding_infos[585];
            break;
        case 2:
            info = &_encoding_infos[596];
            break;
        case 3:
            info = &_encoding_infos[600];
            break;
        case 4:
            info = &_encoding_infos[604];
            break;
        case 5:
            info = &_encoding_infos[617];
            break;
        case 6:
            info = &_encoding_infos[631];
            break;
        }
        break;
    case kEncoding_pwm_duty_cycle:
        switch (value) {
        case 0 ... 100:
            info = &_encoding_infos[637];
            break;
        }
        break;
    case kEncoding_RPM:
        switch (value) {
        case 0 ... 255:
            info = &_encoding_infos[638];
            break;
        }
        break;
    case kEncoding_PSI:
        switch (value) {
        case 0 ... 255:
            info = &_encoding_infos[639];
            break;
        }
        break;
    case kEncoding_F:
        switch (value) {
        case 0 ... 255:
            info = &_encoding_infos[640];
            break;
        }
        break;
    case kEncoding_V:
        switch (value) {
        case 0 ... 255:
            info = &_encoding_infos[641];
            break;
        }
        break;
    case kEncoding_lambda:
        switch (value) {
        case 0 ... 255:
            info = &_encoding_infos[642];
            break;
        }
        break;
    case kEncoding_MPH:
        switch (value) {
        case 0 ... 255:
            info = &_encoding_infos[643];
            break;
        }
        break;
    }
    return info;
}

namespace Generic
{

void param_default(const Parameter &param)
{
    uint16_t value;
    switch (param.address()) {
    default:
        return;
    }
    param.set(value);
}



const PROGMEM char *
param_name(const Parameter &param)
{

}


} // namespace Generic

namespace Master
{

void param_default(const Parameter &param)
{
    uint16_t value;
    switch (param.address()) {
        case kParamSP1Assign:
            value = 0;
            break;
        case kParamSP2Assign:
            value = 0;
            break;
        case kParamSP3Assign:
            value = 0;
            break;
        case kParamSP4Assign:
            value = 0;
            break;
        case kParamSP5Assign:
            value = 0;
            break;
        case kParamSP6Assign:
            value = 0;
            break;
        case kParamSP7Assign:
            value = 0;
            break;
        case kParamSG0Assign:
            value = 0;
            break;
        case kParamSG1Assign:
            value = 0;
            break;
        case kParamSG2Assign:
            value = 0;
            break;
        case kParamSG3Assign:
            value = 0;
            break;
        case kParamSG4Assign:
            value = 0;
            break;
        case kParamSG5Assign:
            value = 0;
            break;
        case kParamSG6Assign:
            value = 0;
            break;
        case kParamSG7Assign:
            value = 0;
            break;
        case kParamSG8Assign:
            value = 0;
            break;
        case kParamSG9Assign:
            value = 0;
            break;
        case kParamSG10Assign:
            value = 0;
            break;
        case kParamSG11Assign:
            value = 0;
            break;
        case kParamSG12Assign:
            value = 0;
            break;
        case kParamSG13Assign:
            value = 0;
            break;
        case kParamTurnBlinkPeriod:
            value = 500;
            break;
        case kParamPassingBlinkCount:
            value = 0;
            break;
        case kParamPathLightTime:
            value = 30;
            break;
        case kParamInteriorLightTime:
            value = 30;
            break;
        case kParamWelcomeLightTime:
            value = 30;
            break;
        case kParamBrakeBlinkPeriod:
            value = 0;
            break;
        case kParamBrakeBlinkCount:
            value = 0;
            break;
        case kParamWiperInterval:
            value = 0;
            break;
    default:
        return;
    }
    param.set(value);
}



const PROGMEM char *
param_name(const Parameter &param)
{

}


} // namespace Master

namespace PowerV1
{

void param_default(const Parameter &param)
{
    uint16_t value;
    switch (param.address()) {
        case kParamRelay1Assign:
            value = 0;
            break;
        case kParamRelay2Assign:
            value = 0;
            break;
        case kParamRelay3Assign:
            value = 0;
            break;
        case kParamRelay4Assign:
            value = 0;
            break;
    default:
        return;
    }
    param.set(value);
}



const PROGMEM char *
param_name(const Parameter &param)
{

}


} // namespace PowerV1

namespace PowerV3
{

void param_default(const Parameter &param)
{
    uint16_t value;
    switch (param.address()) {
        case kParamCH1Type:
            value = 0;
            break;
        case kParamCH2Type:
            value = 0;
            break;
        case kParamCH3Type:
            value = 0;
            break;
        case kParamCH4Type:
            value = 0;
            break;
        case kParamCH5Type:
            value = 0;
            break;
        case kParamCH1Assign1:
            value = 255;
            break;
        case kParamCH1PWM1:
            value = 0;
            break;
        case kParamCH1Assign2:
            value = 255;
            break;
        case kParamCH1PWM2:
            value = 0;
            break;
        case kParamCH1Assign3:
            value = 255;
            break;
        case kParamCH1PWM3:
            value = 0;
            break;
        case kParamCH1Assign4:
            value = 255;
            break;
        case kParamCH1PWM4:
            value = 0;
            break;
        case kParamCH2Assign1:
            value = 255;
            break;
        case kParamCH2PWM1:
            value = 0;
            break;
        case kParamCH2Assign2:
            value = 255;
            break;
        case kParamCH2PWM2:
            value = 0;
            break;
        case kParamCH2Assign3:
            value = 255;
            break;
        case kParamCH2PWM3:
            value = 0;
            break;
        case kParamCH2Assign4:
            value = 255;
            break;
        case kParamCH2PWM4:
            value = 0;
            break;
        case kParamCH3Assign1:
            value = 255;
            break;
        case kParamCH3PWM1:
            value = 0;
            break;
        case kParamCH3Assign2:
            value = 255;
            break;
        case kParamCH3PWM2:
            value = 0;
            break;
        case kParamCH3Assign3:
            value = 255;
            break;
        case kParamCH3PWM3:
            value = 0;
            break;
        case kParamCH3Assign4:
            value = 255;
            break;
        case kParamCH3PWM4:
            value = 0;
            break;
        case kParamCH4Assign1:
            value = 255;
            break;
        case kParamCH4PWM1:
            value = 0;
            break;
        case kParamCH4Assign2:
            value = 255;
            break;
        case kParamCH4PWM2:
            value = 0;
            break;
        case kParamCH4Assign3:
            value = 255;
            break;
        case kParamCH4PWM3:
            value = 0;
            break;
        case kParamCH4Assign4:
            value = 255;
            break;
        case kParamCH4PWM4:
            value = 0;
            break;
        case kParamCH5Assign1:
            value = 255;
            break;
        case kParamCH5PWM1:
            value = 0;
            break;
        case kParamCH5Assign2:
            value = 255;
            break;
        case kParamCH5PWM2:
            value = 0;
            break;
        case kParamCH5Assign3:
            value = 255;
            break;
        case kParamCH5PWM3:
            value = 0;
            break;
        case kParamCH5Assign4:
            value = 255;
            break;
        case kParamCH5PWM4:
            value = 0;
            break;
    default:
        return;
    }
    param.set(value);
}



const PROGMEM char *
param_name(const Parameter &param)
{

}


} // namespace PowerV3
