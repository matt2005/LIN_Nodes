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
    case kEncoding_v1_output_status:
        switch (value) {
        case 0:
        case 1:
            return false;
        }
        break;
    case kEncoding_v1_output_assignment:
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
    case kEncoding_v3_output_status:
        switch (value) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
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

static const PROGMEM char _encoding_name_input[] = "input";
static const PROGMEM char _encoding_info_input_0[] = "Unassigned";
static const PROGMEM char _encoding_info_input_1[] = "Ignition";
static const PROGMEM char _encoding_info_input_2[] = "Start";
static const PROGMEM char _encoding_info_input_3[] = "MarkerLights";
static const PROGMEM char _encoding_info_input_4[] = "HeadLights";
static const PROGMEM char _encoding_info_input_5[] = "HighBeam";
static const PROGMEM char _encoding_info_input_6[] = "HighBeamToggle";
static const PROGMEM char _encoding_info_input_7[] = "FogLight";
static const PROGMEM char _encoding_info_input_8[] = "LeftTurn";
static const PROGMEM char _encoding_info_input_9[] = "RightTurn";
static const PROGMEM char _encoding_info_input_10[] = "Brake";
static const PROGMEM char _encoding_info_input_11[] = "Reverse";
static const PROGMEM char _encoding_info_input_12[] = "Door";
static const PROGMEM char _encoding_info_input_13[] = "InteriorLight";
static const PROGMEM char _encoding_info_input_14[] = "Hazard";
static const PROGMEM char _encoding_info_input_15[] = "DoorUnlock";
static const PROGMEM char _encoding_info_input_16[] = "LightsUp";
static const PROGMEM char _encoding_info_input_17[] = "CabinFan1";
static const PROGMEM char _encoding_info_input_18[] = "CabinFan2";
static const PROGMEM char _encoding_info_input_19[] = "CabinFan3";
static const PROGMEM char _encoding_info_input_20[] = "WiperInt";
static const PROGMEM char _encoding_info_input_21[] = "WiperLow";
static const PROGMEM char _encoding_info_input_22[] = "WiperHigh";
static const PROGMEM char _encoding_info_input_23[] = "RearDefrost";
static const PROGMEM char _encoding_name_msec[] = "msec";
static const PROGMEM char _encoding_info_msec_0[] = "Disabled";
static const PROGMEM char _encoding_info_msec_1[] = "";
static const PROGMEM char _encoding_name_sec[] = "sec";
static const PROGMEM char _encoding_info_sec_0[] = "Disabled";
static const PROGMEM char _encoding_info_sec_1[] = "";
static const PROGMEM char _encoding_name_up_to_10[] = "up_to_10";
static const PROGMEM char _encoding_info_up_to_10_0[] = "Disabled";
static const PROGMEM char _encoding_info_up_to_10_1[] = "";
static const PROGMEM char _encoding_name_node_address[] = "node_address";
static const PROGMEM char _encoding_info_node_address_0[] = "Sleep";
static const PROGMEM char _encoding_info_node_address_1[] = "";
static const PROGMEM char _encoding_info_node_address_2[] = "Functional";
static const PROGMEM char _encoding_info_node_address_3[] = "Broadcast";
static const PROGMEM char _encoding_name_pci[] = "pci";
static const PROGMEM char _encoding_info_pci_0[] = "SingleFrame";
static const PROGMEM char _encoding_info_pci_1[] = "FirstFrame";
static const PROGMEM char _encoding_info_pci_2[] = "ContiuationFrame";
static const PROGMEM char _encoding_name_service_id[] = "service_id";
static const PROGMEM char _encoding_info_service_id_0[] = "ReadDataByID";
static const PROGMEM char _encoding_info_service_id_1[] = "WriteDataByID";
static const PROGMEM char _encoding_info_service_id_2[] = "TesterPresent";
static const PROGMEM char _encoding_info_service_id_3[] = "ReadByID";
static const PROGMEM char _encoding_info_service_id_4[] = "DataDump";
static const PROGMEM char _encoding_info_service_id_5[] = "ErrorResponse";
static const PROGMEM char _encoding_info_service_id_6[] = "ResponseOffset";
static const PROGMEM char _encoding_name_service_error[] = "service_error";
static const PROGMEM char _encoding_info_service_error_0[] = "FunctionNotSupported";
static const PROGMEM char _encoding_info_service_error_1[] = "IncorrectLength";
static const PROGMEM char _encoding_info_service_error_2[] = "ConditionsNotCorrect";
static const PROGMEM char _encoding_info_service_error_3[] = "OutOfRange";
static const PROGMEM char _encoding_info_service_error_4[] = "AccessDenied";
static const PROGMEM char _encoding_info_service_error_5[] = "GeneralFailure";
static const PROGMEM char _encoding_name_v1_output_status[] = "v1_output_status";
static const PROGMEM char _encoding_info_v1_output_status_0[] = "OK";
static const PROGMEM char _encoding_info_v1_output_status_1[] = "Fault";
static const PROGMEM char _encoding_name_v1_output_assignment[] = "v1_output_assignment";
static const PROGMEM char _encoding_info_v1_output_assignment_0[] = "Unassigned";
static const PROGMEM char _encoding_info_v1_output_assignment_1[] = "Ignition";
static const PROGMEM char _encoding_info_v1_output_assignment_2[] = "Start";
static const PROGMEM char _encoding_info_v1_output_assignment_3[] = "LightsUp";
static const PROGMEM char _encoding_info_v1_output_assignment_4[] = "LightsDown";
static const PROGMEM char _encoding_info_v1_output_assignment_5[] = "HeadLights";
static const PROGMEM char _encoding_info_v1_output_assignment_6[] = "LowBeam";
static const PROGMEM char _encoding_info_v1_output_assignment_7[] = "HighBeam";
static const PROGMEM char _encoding_info_v1_output_assignment_8[] = "FogLights";
static const PROGMEM char _encoding_info_v1_output_assignment_9[] = "MarkerLights";
static const PROGMEM char _encoding_info_v1_output_assignment_10[] = "LeftTurn";
static const PROGMEM char _encoding_info_v1_output_assignment_11[] = "LeftTurnMarker";
static const PROGMEM char _encoding_info_v1_output_assignment_12[] = "RightTurn";
static const PROGMEM char _encoding_info_v1_output_assignment_13[] = "RightTurnMarker";
static const PROGMEM char _encoding_info_v1_output_assignment_14[] = "Brake";
static const PROGMEM char _encoding_info_v1_output_assignment_15[] = "Reverse";
static const PROGMEM char _encoding_info_v1_output_assignment_16[] = "InteriorLight";
static const PROGMEM char _encoding_info_v1_output_assignment_17[] = "CabinFan1";
static const PROGMEM char _encoding_info_v1_output_assignment_18[] = "CabinFan2";
static const PROGMEM char _encoding_info_v1_output_assignment_19[] = "CabinFan3";
static const PROGMEM char _encoding_info_v1_output_assignment_20[] = "CabinFan4";
static const PROGMEM char _encoding_info_v1_output_assignment_21[] = "WiperLow";
static const PROGMEM char _encoding_info_v1_output_assignment_22[] = "WiperHigh";
static const PROGMEM char _encoding_info_v1_output_assignment_23[] = "RearDefrost";
static const PROGMEM char _encoding_name_v3_output_status[] = "v3_output_status";
static const PROGMEM char _encoding_info_v3_output_status_0[] = "OK";
static const PROGMEM char _encoding_info_v3_output_status_1[] = "OpenCircuit";
static const PROGMEM char _encoding_info_v3_output_status_2[] = "OverCurrent";
static const PROGMEM char _encoding_info_v3_output_status_3[] = "OverTemperature";
static const PROGMEM char _encoding_info_v3_output_status_4[] = "ShortToGround";
static const PROGMEM char _encoding_info_v3_output_status_5[] = "ShortToBattery";
static const PROGMEM char _encoding_name_output_type[] = "output_type";
static const PROGMEM char _encoding_info_output_type_0[] = "5AGeneric";
static const PROGMEM char _encoding_info_output_type_1[] = "10AGeneric";
static const PROGMEM char _encoding_info_output_type_2[] = "LED";
static const PROGMEM char _encoding_info_output_type_3[] = "HID";
static const PROGMEM char _encoding_info_output_type_4[] = "LowPowerBulb";
static const PROGMEM char _encoding_info_output_type_5[] = "HighPowerBulb";
static const PROGMEM char _encoding_info_output_type_6[] = "Motor";
static const PROGMEM char _encoding_name_output_assignment[] = "output_assignment";
static const PROGMEM char _encoding_info_output_assignment_0[] = "Unassigned";
static const PROGMEM char _encoding_info_output_assignment_1[] = "Ignition";
static const PROGMEM char _encoding_info_output_assignment_2[] = "Start";
static const PROGMEM char _encoding_info_output_assignment_3[] = "LightsUp";
static const PROGMEM char _encoding_info_output_assignment_4[] = "LightsDown";
static const PROGMEM char _encoding_info_output_assignment_5[] = "HeadLights";
static const PROGMEM char _encoding_info_output_assignment_6[] = "LowBeam";
static const PROGMEM char _encoding_info_output_assignment_7[] = "HighBeam";
static const PROGMEM char _encoding_info_output_assignment_8[] = "FogLights";
static const PROGMEM char _encoding_info_output_assignment_9[] = "MarkerLights";
static const PROGMEM char _encoding_info_output_assignment_10[] = "LeftTurn";
static const PROGMEM char _encoding_info_output_assignment_11[] = "LeftTurnMarker";
static const PROGMEM char _encoding_info_output_assignment_12[] = "RightTurn";
static const PROGMEM char _encoding_info_output_assignment_13[] = "RightTurnMarker";
static const PROGMEM char _encoding_info_output_assignment_14[] = "Brake";
static const PROGMEM char _encoding_info_output_assignment_15[] = "Reverse";
static const PROGMEM char _encoding_info_output_assignment_16[] = "InteriorLight";
static const PROGMEM char _encoding_info_output_assignment_17[] = "CabinFan1";
static const PROGMEM char _encoding_info_output_assignment_18[] = "CabinFan2";
static const PROGMEM char _encoding_info_output_assignment_19[] = "CabinFan3";
static const PROGMEM char _encoding_info_output_assignment_20[] = "CabinFan4";
static const PROGMEM char _encoding_info_output_assignment_21[] = "WiperLow";
static const PROGMEM char _encoding_info_output_assignment_22[] = "WiperHigh";
static const PROGMEM char _encoding_info_output_assignment_23[] = "RearDefrost";
static const PROGMEM char _encoding_name_pwm_duty_cycle[] = "pwm_duty_cycle";
static const PROGMEM char _encoding_info_pwm_duty_cycle_0[] = "";
static const PROGMEM char _encoding_name_RPM[] = "RPM";
static const PROGMEM char _encoding_info_RPM_0[] = "";
static const PROGMEM char _encoding_name_PSI[] = "PSI";
static const PROGMEM char _encoding_info_PSI_0[] = "";
static const PROGMEM char _encoding_name_F[] = "F";
static const PROGMEM char _encoding_info_F_0[] = "";
static const PROGMEM char _encoding_name_V[] = "V";
static const PROGMEM char _encoding_info_V_0[] = "";
static const PROGMEM char _encoding_name_lambda[] = "lambda";
static const PROGMEM char _encoding_info_lambda_0[] = "";
static const PROGMEM char _encoding_name_MPH[] = "MPH";
static const PROGMEM char _encoding_info_MPH_0[] = "";

const PROGMEM char *
Encoding::name(uint8_t index)
{
    switch (index) {
    case kEncoding_input:
        return &_encoding_name_input[0];
    case kEncoding_msec:
        return &_encoding_name_msec[0];
    case kEncoding_sec:
        return &_encoding_name_sec[0];
    case kEncoding_up_to_10:
        return &_encoding_name_up_to_10[0];
    case kEncoding_node_address:
        return &_encoding_name_node_address[0];
    case kEncoding_pci:
        return &_encoding_name_pci[0];
    case kEncoding_service_id:
        return &_encoding_name_service_id[0];
    case kEncoding_service_error:
        return &_encoding_name_service_error[0];
    case kEncoding_v1_output_status:
        return &_encoding_name_v1_output_status[0];
    case kEncoding_v1_output_assignment:
        return &_encoding_name_v1_output_assignment[0];
    case kEncoding_v3_output_status:
        return &_encoding_name_v3_output_status[0];
    case kEncoding_output_type:
        return &_encoding_name_output_type[0];
    case kEncoding_output_assignment:
        return &_encoding_name_output_assignment[0];
    case kEncoding_pwm_duty_cycle:
        return &_encoding_name_pwm_duty_cycle[0];
    case kEncoding_RPM:
        return &_encoding_name_RPM[0];
    case kEncoding_PSI:
        return &_encoding_name_PSI[0];
    case kEncoding_F:
        return &_encoding_name_F[0];
    case kEncoding_V:
        return &_encoding_name_V[0];
    case kEncoding_lambda:
        return &_encoding_name_lambda[0];
    case kEncoding_MPH:
        return &_encoding_name_MPH[0];
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
            info = &_encoding_info_input_0[0];
            break;
        case 1:
            info = &_encoding_info_input_1[0];
            break;
        case 2:
            info = &_encoding_info_input_2[0];
            break;
        case 3:
            info = &_encoding_info_input_3[0];
            break;
        case 4:
            info = &_encoding_info_input_4[0];
            break;
        case 5:
            info = &_encoding_info_input_5[0];
            break;
        case 6:
            info = &_encoding_info_input_6[0];
            break;
        case 7:
            info = &_encoding_info_input_7[0];
            break;
        case 8:
            info = &_encoding_info_input_8[0];
            break;
        case 9:
            info = &_encoding_info_input_9[0];
            break;
        case 10:
            info = &_encoding_info_input_10[0];
            break;
        case 11:
            info = &_encoding_info_input_11[0];
            break;
        case 12:
            info = &_encoding_info_input_12[0];
            break;
        case 13:
            info = &_encoding_info_input_13[0];
            break;
        case 14:
            info = &_encoding_info_input_14[0];
            break;
        case 15:
            info = &_encoding_info_input_15[0];
            break;
        case 16:
            info = &_encoding_info_input_16[0];
            break;
        case 17:
            info = &_encoding_info_input_17[0];
            break;
        case 18:
            info = &_encoding_info_input_18[0];
            break;
        case 19:
            info = &_encoding_info_input_19[0];
            break;
        case 20:
            info = &_encoding_info_input_20[0];
            break;
        case 21:
            info = &_encoding_info_input_21[0];
            break;
        case 22:
            info = &_encoding_info_input_22[0];
            break;
        case 23:
            info = &_encoding_info_input_23[0];
            break;
        }
        break;
    case kEncoding_msec:
        switch (value) {
        case 0:
            info = &_encoding_info_msec_0[0];
            break;
        case 100 ... 65000:
            info = &_encoding_info_msec_1[0];
            break;
        }
        break;
    case kEncoding_sec:
        switch (value) {
        case 0:
            info = &_encoding_info_sec_0[0];
            break;
        case 1 ... 600:
            info = &_encoding_info_sec_1[0];
            break;
        }
        break;
    case kEncoding_up_to_10:
        switch (value) {
        case 0:
            info = &_encoding_info_up_to_10_0[0];
            break;
        case 1 ... 10:
            info = &_encoding_info_up_to_10_1[0];
            break;
        }
        break;
    case kEncoding_node_address:
        switch (value) {
        case 0:
            info = &_encoding_info_node_address_0[0];
            break;
        case 1 ... 125:
            info = &_encoding_info_node_address_1[0];
            break;
        case 126:
            info = &_encoding_info_node_address_2[0];
            break;
        case 127:
            info = &_encoding_info_node_address_3[0];
            break;
        }
        break;
    case kEncoding_pci:
        switch (value) {
        case 0:
            info = &_encoding_info_pci_0[0];
            break;
        case 1:
            info = &_encoding_info_pci_1[0];
            break;
        case 2:
            info = &_encoding_info_pci_2[0];
            break;
        }
        break;
    case kEncoding_service_id:
        switch (value) {
        case 0x22:
            info = &_encoding_info_service_id_0[0];
            break;
        case 0x2e:
            info = &_encoding_info_service_id_1[0];
            break;
        case 0x3e:
            info = &_encoding_info_service_id_2[0];
            break;
        case 0xb2:
            info = &_encoding_info_service_id_3[0];
            break;
        case 0xb4:
            info = &_encoding_info_service_id_4[0];
            break;
        case 0x7f:
            info = &_encoding_info_service_id_5[0];
            break;
        case 0x40:
            info = &_encoding_info_service_id_6[0];
            break;
        }
        break;
    case kEncoding_service_error:
        switch (value) {
        case 0x12:
            info = &_encoding_info_service_error_0[0];
            break;
        case 0x13:
            info = &_encoding_info_service_error_1[0];
            break;
        case 0x22:
            info = &_encoding_info_service_error_2[0];
            break;
        case 0x31:
            info = &_encoding_info_service_error_3[0];
            break;
        case 0x33:
            info = &_encoding_info_service_error_4[0];
            break;
        case 0x72:
            info = &_encoding_info_service_error_5[0];
            break;
        }
        break;
    case kEncoding_v1_output_status:
        switch (value) {
        case 0:
            info = &_encoding_info_v1_output_status_0[0];
            break;
        case 1:
            info = &_encoding_info_v1_output_status_1[0];
            break;
        }
        break;
    case kEncoding_v1_output_assignment:
        switch (value) {
        case 0:
            info = &_encoding_info_v1_output_assignment_0[0];
            break;
        case 1:
            info = &_encoding_info_v1_output_assignment_1[0];
            break;
        case 2:
            info = &_encoding_info_v1_output_assignment_2[0];
            break;
        case 3:
            info = &_encoding_info_v1_output_assignment_3[0];
            break;
        case 4:
            info = &_encoding_info_v1_output_assignment_4[0];
            break;
        case 5:
            info = &_encoding_info_v1_output_assignment_5[0];
            break;
        case 6:
            info = &_encoding_info_v1_output_assignment_6[0];
            break;
        case 7:
            info = &_encoding_info_v1_output_assignment_7[0];
            break;
        case 8:
            info = &_encoding_info_v1_output_assignment_8[0];
            break;
        case 9:
            info = &_encoding_info_v1_output_assignment_9[0];
            break;
        case 10:
            info = &_encoding_info_v1_output_assignment_10[0];
            break;
        case 11:
            info = &_encoding_info_v1_output_assignment_11[0];
            break;
        case 12:
            info = &_encoding_info_v1_output_assignment_12[0];
            break;
        case 13:
            info = &_encoding_info_v1_output_assignment_13[0];
            break;
        case 14:
            info = &_encoding_info_v1_output_assignment_14[0];
            break;
        case 15:
            info = &_encoding_info_v1_output_assignment_15[0];
            break;
        case 16:
            info = &_encoding_info_v1_output_assignment_16[0];
            break;
        case 17:
            info = &_encoding_info_v1_output_assignment_17[0];
            break;
        case 18:
            info = &_encoding_info_v1_output_assignment_18[0];
            break;
        case 19:
            info = &_encoding_info_v1_output_assignment_19[0];
            break;
        case 20:
            info = &_encoding_info_v1_output_assignment_20[0];
            break;
        case 21:
            info = &_encoding_info_v1_output_assignment_21[0];
            break;
        case 22:
            info = &_encoding_info_v1_output_assignment_22[0];
            break;
        case 23:
            info = &_encoding_info_v1_output_assignment_23[0];
            break;
        }
        break;
    case kEncoding_v3_output_status:
        switch (value) {
        case 0:
            info = &_encoding_info_v3_output_status_0[0];
            break;
        case 1:
            info = &_encoding_info_v3_output_status_1[0];
            break;
        case 2:
            info = &_encoding_info_v3_output_status_2[0];
            break;
        case 3:
            info = &_encoding_info_v3_output_status_3[0];
            break;
        case 4:
            info = &_encoding_info_v3_output_status_4[0];
            break;
        case 5:
            info = &_encoding_info_v3_output_status_5[0];
            break;
        }
        break;
    case kEncoding_output_type:
        switch (value) {
        case 0:
            info = &_encoding_info_output_type_0[0];
            break;
        case 1:
            info = &_encoding_info_output_type_1[0];
            break;
        case 2:
            info = &_encoding_info_output_type_2[0];
            break;
        case 3:
            info = &_encoding_info_output_type_3[0];
            break;
        case 4:
            info = &_encoding_info_output_type_4[0];
            break;
        case 5:
            info = &_encoding_info_output_type_5[0];
            break;
        case 6:
            info = &_encoding_info_output_type_6[0];
            break;
        }
        break;
    case kEncoding_output_assignment:
        switch (value) {
        case 0:
            info = &_encoding_info_output_assignment_0[0];
            break;
        case 1:
            info = &_encoding_info_output_assignment_1[0];
            break;
        case 2:
            info = &_encoding_info_output_assignment_2[0];
            break;
        case 3:
            info = &_encoding_info_output_assignment_3[0];
            break;
        case 4:
            info = &_encoding_info_output_assignment_4[0];
            break;
        case 5:
            info = &_encoding_info_output_assignment_5[0];
            break;
        case 6:
            info = &_encoding_info_output_assignment_6[0];
            break;
        case 7:
            info = &_encoding_info_output_assignment_7[0];
            break;
        case 8:
            info = &_encoding_info_output_assignment_8[0];
            break;
        case 9:
            info = &_encoding_info_output_assignment_9[0];
            break;
        case 10:
            info = &_encoding_info_output_assignment_10[0];
            break;
        case 11:
            info = &_encoding_info_output_assignment_11[0];
            break;
        case 12:
            info = &_encoding_info_output_assignment_12[0];
            break;
        case 13:
            info = &_encoding_info_output_assignment_13[0];
            break;
        case 14:
            info = &_encoding_info_output_assignment_14[0];
            break;
        case 15:
            info = &_encoding_info_output_assignment_15[0];
            break;
        case 16:
            info = &_encoding_info_output_assignment_16[0];
            break;
        case 17:
            info = &_encoding_info_output_assignment_17[0];
            break;
        case 18:
            info = &_encoding_info_output_assignment_18[0];
            break;
        case 19:
            info = &_encoding_info_output_assignment_19[0];
            break;
        case 20:
            info = &_encoding_info_output_assignment_20[0];
            break;
        case 21:
            info = &_encoding_info_output_assignment_21[0];
            break;
        case 22:
            info = &_encoding_info_output_assignment_22[0];
            break;
        case 23:
            info = &_encoding_info_output_assignment_23[0];
            break;
        }
        break;
    case kEncoding_pwm_duty_cycle:
        switch (value) {
        case 0 ... 100:
            info = &_encoding_info_pwm_duty_cycle_0[0];
            break;
        }
        break;
    case kEncoding_RPM:
        switch (value) {
        case 0 ... 255:
            info = &_encoding_info_RPM_0[0];
            break;
        }
        break;
    case kEncoding_PSI:
        switch (value) {
        case 0 ... 255:
            info = &_encoding_info_PSI_0[0];
            break;
        }
        break;
    case kEncoding_F:
        switch (value) {
        case 0 ... 255:
            info = &_encoding_info_F_0[0];
            break;
        }
        break;
    case kEncoding_V:
        switch (value) {
        case 0 ... 255:
            info = &_encoding_info_V_0[0];
            break;
        }
        break;
    case kEncoding_lambda:
        switch (value) {
        case 0 ... 255:
            info = &_encoding_info_lambda_0[0];
            break;
        }
        break;
    case kEncoding_MPH:
        switch (value) {
        case 0 ... 255:
            info = &_encoding_info_MPH_0[0];
            break;
        }
        break;
    }
    return info;
}

namespace Generic
{

// parameter factory for Generic
Parameter
parameter(Parameter::Address address)
{
    uint8_t encoding = kEncoding_none;
    switch (address) {
    case kParamProtocolVersion:
        break;
    case kParamBoardFunction:
        break;
    case kParamBootloaderMode:
        break;
    case kParamFirmwareVersion:
        break;
    case kParamFirmwarePageSize:
        break;
    case kParamWatchdogResets:
        break;
    case kParamSupplyVoltage:
        break;
    case kParamTemperature:
        break;
    case kParamLine:
        break;
    case kParamChecksum:
        break;
    case kParamParity:
        break;
    case kParamFraming:
        break;
    case kParamSynch:
        break;
    case kParamProtocol:
        break;
    case kParamConfigBase:
        break;
    default:
        address = Parameter::noAddress;
    }
    return Parameter(address, encoding, param_default, param_name);
}

void param_default(const Parameter &param)
{
    uint16_t value;
    switch (param.address()) {
    default:
        return;
    }
    param.set(value);
}

static const PROGMEM char _param_name_ProtocolVersion[] = "ProtocolVersion";
static const PROGMEM char _param_name_BoardFunction[] = "BoardFunction";
static const PROGMEM char _param_name_BootloaderMode[] = "BootloaderMode";
static const PROGMEM char _param_name_FirmwareVersion[] = "FirmwareVersion";
static const PROGMEM char _param_name_FirmwarePageSize[] = "FirmwarePageSize";
static const PROGMEM char _param_name_WatchdogResets[] = "WatchdogResets";
static const PROGMEM char _param_name_SupplyVoltage[] = "SupplyVoltage";
static const PROGMEM char _param_name_Temperature[] = "Temperature";
static const PROGMEM char _param_name_Line[] = "Line";
static const PROGMEM char _param_name_Checksum[] = "Checksum";
static const PROGMEM char _param_name_Parity[] = "Parity";
static const PROGMEM char _param_name_Framing[] = "Framing";
static const PROGMEM char _param_name_Synch[] = "Synch";
static const PROGMEM char _param_name_Protocol[] = "Protocol";
static const PROGMEM char _param_name_ConfigBase[] = "ConfigBase";

const PROGMEM char *
param_name(const Parameter &param)
{
    switch (param.address()) {
    case kParamProtocolVersion:
        return &_param_name_ProtocolVersion[0];
    case kParamBoardFunction:
        return &_param_name_BoardFunction[0];
    case kParamBootloaderMode:
        return &_param_name_BootloaderMode[0];
    case kParamFirmwareVersion:
        return &_param_name_FirmwareVersion[0];
    case kParamFirmwarePageSize:
        return &_param_name_FirmwarePageSize[0];
    case kParamWatchdogResets:
        return &_param_name_WatchdogResets[0];
    case kParamSupplyVoltage:
        return &_param_name_SupplyVoltage[0];
    case kParamTemperature:
        return &_param_name_Temperature[0];
    case kParamLine:
        return &_param_name_Line[0];
    case kParamChecksum:
        return &_param_name_Checksum[0];
    case kParamParity:
        return &_param_name_Parity[0];
    case kParamFraming:
        return &_param_name_Framing[0];
    case kParamSynch:
        return &_param_name_Synch[0];
    case kParamProtocol:
        return &_param_name_Protocol[0];
    case kParamConfigBase:
        return &_param_name_ConfigBase[0];
    }
    return nullptr;
}


} // namespace Generic

namespace Master
{

// parameter factory for Master
Parameter
parameter(Parameter::Address address)
{
    uint8_t encoding = kEncoding_none;
    switch (address) {
    case kParamSP1Assign:
        encoding = kEncoding_input;
        break;
    case kParamSP2Assign:
        encoding = kEncoding_input;
        break;
    case kParamSP3Assign:
        encoding = kEncoding_input;
        break;
    case kParamSP4Assign:
        encoding = kEncoding_input;
        break;
    case kParamSP5Assign:
        encoding = kEncoding_input;
        break;
    case kParamSP6Assign:
        encoding = kEncoding_input;
        break;
    case kParamSP7Assign:
        encoding = kEncoding_input;
        break;
    case kParamSG0Assign:
        encoding = kEncoding_input;
        break;
    case kParamSG1Assign:
        encoding = kEncoding_input;
        break;
    case kParamSG2Assign:
        encoding = kEncoding_input;
        break;
    case kParamSG3Assign:
        encoding = kEncoding_input;
        break;
    case kParamSG4Assign:
        encoding = kEncoding_input;
        break;
    case kParamSG5Assign:
        encoding = kEncoding_input;
        break;
    case kParamSG6Assign:
        encoding = kEncoding_input;
        break;
    case kParamSG7Assign:
        encoding = kEncoding_input;
        break;
    case kParamSG8Assign:
        encoding = kEncoding_input;
        break;
    case kParamSG9Assign:
        encoding = kEncoding_input;
        break;
    case kParamSG10Assign:
        encoding = kEncoding_input;
        break;
    case kParamSG11Assign:
        encoding = kEncoding_input;
        break;
    case kParamSG12Assign:
        encoding = kEncoding_input;
        break;
    case kParamSG13Assign:
        encoding = kEncoding_input;
        break;
    case kParamTurnBlinkPeriod:
        encoding = kEncoding_msec;
        break;
    case kParamPassingBlinkCount:
        encoding = kEncoding_up_to_10;
        break;
    case kParamPathLightTime:
        encoding = kEncoding_sec;
        break;
    case kParamInteriorLightTime:
        encoding = kEncoding_sec;
        break;
    case kParamWelcomeLightTime:
        encoding = kEncoding_sec;
        break;
    case kParamBrakeBlinkPeriod:
        encoding = kEncoding_msec;
        break;
    case kParamBrakeBlinkCount:
        encoding = kEncoding_up_to_10;
        break;
    case kParamWiperInterval:
        encoding = kEncoding_msec;
        break;
    default:
        address = Parameter::noAddress;
    }
    return Parameter(address, encoding, param_default, param_name);
}

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

static const PROGMEM char _param_name_SP1Assign[] = "SP1Assign";
static const PROGMEM char _param_name_SP2Assign[] = "SP2Assign";
static const PROGMEM char _param_name_SP3Assign[] = "SP3Assign";
static const PROGMEM char _param_name_SP4Assign[] = "SP4Assign";
static const PROGMEM char _param_name_SP5Assign[] = "SP5Assign";
static const PROGMEM char _param_name_SP6Assign[] = "SP6Assign";
static const PROGMEM char _param_name_SP7Assign[] = "SP7Assign";
static const PROGMEM char _param_name_SG0Assign[] = "SG0Assign";
static const PROGMEM char _param_name_SG1Assign[] = "SG1Assign";
static const PROGMEM char _param_name_SG2Assign[] = "SG2Assign";
static const PROGMEM char _param_name_SG3Assign[] = "SG3Assign";
static const PROGMEM char _param_name_SG4Assign[] = "SG4Assign";
static const PROGMEM char _param_name_SG5Assign[] = "SG5Assign";
static const PROGMEM char _param_name_SG6Assign[] = "SG6Assign";
static const PROGMEM char _param_name_SG7Assign[] = "SG7Assign";
static const PROGMEM char _param_name_SG8Assign[] = "SG8Assign";
static const PROGMEM char _param_name_SG9Assign[] = "SG9Assign";
static const PROGMEM char _param_name_SG10Assign[] = "SG10Assign";
static const PROGMEM char _param_name_SG11Assign[] = "SG11Assign";
static const PROGMEM char _param_name_SG12Assign[] = "SG12Assign";
static const PROGMEM char _param_name_SG13Assign[] = "SG13Assign";
static const PROGMEM char _param_name_TurnBlinkPeriod[] = "TurnBlinkPeriod";
static const PROGMEM char _param_name_PassingBlinkCount[] = "PassingBlinkCount";
static const PROGMEM char _param_name_PathLightTime[] = "PathLightTime";
static const PROGMEM char _param_name_InteriorLightTime[] = "InteriorLightTime";
static const PROGMEM char _param_name_WelcomeLightTime[] = "WelcomeLightTime";
static const PROGMEM char _param_name_BrakeBlinkPeriod[] = "BrakeBlinkPeriod";
static const PROGMEM char _param_name_BrakeBlinkCount[] = "BrakeBlinkCount";
static const PROGMEM char _param_name_WiperInterval[] = "WiperInterval";

const PROGMEM char *
param_name(const Parameter &param)
{
    switch (param.address()) {
    case kParamSP1Assign:
        return &_param_name_SP1Assign[0];
    case kParamSP2Assign:
        return &_param_name_SP2Assign[0];
    case kParamSP3Assign:
        return &_param_name_SP3Assign[0];
    case kParamSP4Assign:
        return &_param_name_SP4Assign[0];
    case kParamSP5Assign:
        return &_param_name_SP5Assign[0];
    case kParamSP6Assign:
        return &_param_name_SP6Assign[0];
    case kParamSP7Assign:
        return &_param_name_SP7Assign[0];
    case kParamSG0Assign:
        return &_param_name_SG0Assign[0];
    case kParamSG1Assign:
        return &_param_name_SG1Assign[0];
    case kParamSG2Assign:
        return &_param_name_SG2Assign[0];
    case kParamSG3Assign:
        return &_param_name_SG3Assign[0];
    case kParamSG4Assign:
        return &_param_name_SG4Assign[0];
    case kParamSG5Assign:
        return &_param_name_SG5Assign[0];
    case kParamSG6Assign:
        return &_param_name_SG6Assign[0];
    case kParamSG7Assign:
        return &_param_name_SG7Assign[0];
    case kParamSG8Assign:
        return &_param_name_SG8Assign[0];
    case kParamSG9Assign:
        return &_param_name_SG9Assign[0];
    case kParamSG10Assign:
        return &_param_name_SG10Assign[0];
    case kParamSG11Assign:
        return &_param_name_SG11Assign[0];
    case kParamSG12Assign:
        return &_param_name_SG12Assign[0];
    case kParamSG13Assign:
        return &_param_name_SG13Assign[0];
    case kParamTurnBlinkPeriod:
        return &_param_name_TurnBlinkPeriod[0];
    case kParamPassingBlinkCount:
        return &_param_name_PassingBlinkCount[0];
    case kParamPathLightTime:
        return &_param_name_PathLightTime[0];
    case kParamInteriorLightTime:
        return &_param_name_InteriorLightTime[0];
    case kParamWelcomeLightTime:
        return &_param_name_WelcomeLightTime[0];
    case kParamBrakeBlinkPeriod:
        return &_param_name_BrakeBlinkPeriod[0];
    case kParamBrakeBlinkCount:
        return &_param_name_BrakeBlinkCount[0];
    case kParamWiperInterval:
        return &_param_name_WiperInterval[0];
    }
    return nullptr;
}


} // namespace Master

namespace PowerV1
{

// parameter factory for PowerV1
Parameter
parameter(Parameter::Address address)
{
    uint8_t encoding = kEncoding_none;
    switch (address) {
    case kParamRelay1Status:
        encoding = kEncoding_v1_output_status;
        break;
    case kParamRelay2Status:
        encoding = kEncoding_v1_output_status;
        break;
    case kParamRelay3Status:
        encoding = kEncoding_v1_output_status;
        break;
    case kParamRelay4Status:
        encoding = kEncoding_v1_output_status;
        break;
    case kParamRelay1Assign:
        encoding = kEncoding_v1_output_assignment;
        break;
    case kParamRelay2Assign:
        encoding = kEncoding_v1_output_assignment;
        break;
    case kParamRelay3Assign:
        encoding = kEncoding_v1_output_assignment;
        break;
    case kParamRelay4Assign:
        encoding = kEncoding_v1_output_assignment;
        break;
    default:
        address = Parameter::noAddress;
    }
    return Parameter(address, encoding, param_default, param_name);
}

void param_default(const Parameter &param)
{
    uint16_t value;
    switch (param.address()) {
        case kParamRelay1Status:
            value = 0;
            break;
        case kParamRelay2Status:
            value = 0;
            break;
        case kParamRelay3Status:
            value = 0;
            break;
        case kParamRelay4Status:
            value = 0;
            break;
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

static const PROGMEM char _param_name_Relay1Status[] = "Relay1Status";
static const PROGMEM char _param_name_Relay2Status[] = "Relay2Status";
static const PROGMEM char _param_name_Relay3Status[] = "Relay3Status";
static const PROGMEM char _param_name_Relay4Status[] = "Relay4Status";
static const PROGMEM char _param_name_Relay1Assign[] = "Relay1Assign";
static const PROGMEM char _param_name_Relay2Assign[] = "Relay2Assign";
static const PROGMEM char _param_name_Relay3Assign[] = "Relay3Assign";
static const PROGMEM char _param_name_Relay4Assign[] = "Relay4Assign";

const PROGMEM char *
param_name(const Parameter &param)
{
    switch (param.address()) {
    case kParamRelay1Status:
        return &_param_name_Relay1Status[0];
    case kParamRelay2Status:
        return &_param_name_Relay2Status[0];
    case kParamRelay3Status:
        return &_param_name_Relay3Status[0];
    case kParamRelay4Status:
        return &_param_name_Relay4Status[0];
    case kParamRelay1Assign:
        return &_param_name_Relay1Assign[0];
    case kParamRelay2Assign:
        return &_param_name_Relay2Assign[0];
    case kParamRelay3Assign:
        return &_param_name_Relay3Assign[0];
    case kParamRelay4Assign:
        return &_param_name_Relay4Assign[0];
    }
    return nullptr;
}


} // namespace PowerV1

namespace PowerV3
{

// parameter factory for PowerV3
Parameter
parameter(Parameter::Address address)
{
    uint8_t encoding = kEncoding_none;
    switch (address) {
    case kParamCH1Status:
        encoding = kEncoding_v3_output_status;
        break;
    case kParamCH2Status:
        encoding = kEncoding_v3_output_status;
        break;
    case kParamCH3Status:
        encoding = kEncoding_v3_output_status;
        break;
    case kParamCH4Status:
        encoding = kEncoding_v3_output_status;
        break;
    case kParamCH5Status:
        encoding = kEncoding_v3_output_status;
        break;
    case kParamCH1Type:
        encoding = kEncoding_output_type;
        break;
    case kParamCH2Type:
        encoding = kEncoding_output_type;
        break;
    case kParamCH3Type:
        encoding = kEncoding_output_type;
        break;
    case kParamCH4Type:
        encoding = kEncoding_output_type;
        break;
    case kParamCH5Type:
        encoding = kEncoding_output_type;
        break;
    case kParamCH1Assign1:
        encoding = kEncoding_output_assignment;
        break;
    case kParamCH1Assign2:
        encoding = kEncoding_output_assignment;
        break;
    case kParamCH1Assign3:
        encoding = kEncoding_output_assignment;
        break;
    case kParamCH1Assign4:
        encoding = kEncoding_output_assignment;
        break;
    case kParamCH1PWM1:
        encoding = kEncoding_pwm_duty_cycle;
        break;
    case kParamCH1PWM2:
        encoding = kEncoding_pwm_duty_cycle;
        break;
    case kParamCH1PWM3:
        encoding = kEncoding_pwm_duty_cycle;
        break;
    case kParamCH1PWM4:
        encoding = kEncoding_pwm_duty_cycle;
        break;
    case kParamCH2Assign1:
        encoding = kEncoding_output_assignment;
        break;
    case kParamCH2Assign2:
        encoding = kEncoding_output_assignment;
        break;
    case kParamCH2Assign3:
        encoding = kEncoding_output_assignment;
        break;
    case kParamCH2Assign4:
        encoding = kEncoding_output_assignment;
        break;
    case kParamCH2PWM1:
        encoding = kEncoding_pwm_duty_cycle;
        break;
    case kParamCH2PWM2:
        encoding = kEncoding_pwm_duty_cycle;
        break;
    case kParamCH2PWM3:
        encoding = kEncoding_pwm_duty_cycle;
        break;
    case kParamCH2PWM4:
        encoding = kEncoding_pwm_duty_cycle;
        break;
    case kParamCH3Assign1:
        encoding = kEncoding_output_assignment;
        break;
    case kParamCH3Assign2:
        encoding = kEncoding_output_assignment;
        break;
    case kParamCH3Assign3:
        encoding = kEncoding_output_assignment;
        break;
    case kParamCH3Assign4:
        encoding = kEncoding_output_assignment;
        break;
    case kParamCH3PWM1:
        encoding = kEncoding_pwm_duty_cycle;
        break;
    case kParamCH3PWM2:
        encoding = kEncoding_pwm_duty_cycle;
        break;
    case kParamCH3PWM3:
        encoding = kEncoding_pwm_duty_cycle;
        break;
    case kParamCH3PWM4:
        encoding = kEncoding_pwm_duty_cycle;
        break;
    case kParamCH4Assign1:
        encoding = kEncoding_output_assignment;
        break;
    case kParamCH4Assign2:
        encoding = kEncoding_output_assignment;
        break;
    case kParamCH4Assign3:
        encoding = kEncoding_output_assignment;
        break;
    case kParamCH4Assign4:
        encoding = kEncoding_output_assignment;
        break;
    case kParamCH4PWM1:
        encoding = kEncoding_pwm_duty_cycle;
        break;
    case kParamCH4PWM2:
        encoding = kEncoding_pwm_duty_cycle;
        break;
    case kParamCH4PWM3:
        encoding = kEncoding_pwm_duty_cycle;
        break;
    case kParamCH4PWM4:
        encoding = kEncoding_pwm_duty_cycle;
        break;
    case kParamCH5Assign1:
        encoding = kEncoding_output_assignment;
        break;
    case kParamCH5Assign2:
        encoding = kEncoding_output_assignment;
        break;
    case kParamCH5Assign3:
        encoding = kEncoding_output_assignment;
        break;
    case kParamCH5Assign4:
        encoding = kEncoding_output_assignment;
        break;
    case kParamCH5PWM1:
        encoding = kEncoding_pwm_duty_cycle;
        break;
    case kParamCH5PWM2:
        encoding = kEncoding_pwm_duty_cycle;
        break;
    case kParamCH5PWM3:
        encoding = kEncoding_pwm_duty_cycle;
        break;
    case kParamCH5PWM4:
        encoding = kEncoding_pwm_duty_cycle;
        break;
    default:
        address = Parameter::noAddress;
    }
    return Parameter(address, encoding, param_default, param_name);
}

void param_default(const Parameter &param)
{
    uint16_t value;
    switch (param.address()) {
        case kParamCH1Status:
            value = 0;
            break;
        case kParamCH2Status:
            value = 0;
            break;
        case kParamCH3Status:
            value = 0;
            break;
        case kParamCH4Status:
            value = 0;
            break;
        case kParamCH5Status:
            value = 0;
            break;
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
        case kParamCH1Assign2:
            value = 255;
            break;
        case kParamCH1Assign3:
            value = 255;
            break;
        case kParamCH1Assign4:
            value = 255;
            break;
        case kParamCH1PWM1:
            value = 0;
            break;
        case kParamCH1PWM2:
            value = 0;
            break;
        case kParamCH1PWM3:
            value = 0;
            break;
        case kParamCH1PWM4:
            value = 0;
            break;
        case kParamCH2Assign1:
            value = 255;
            break;
        case kParamCH2Assign2:
            value = 255;
            break;
        case kParamCH2Assign3:
            value = 255;
            break;
        case kParamCH2Assign4:
            value = 255;
            break;
        case kParamCH2PWM1:
            value = 0;
            break;
        case kParamCH2PWM2:
            value = 0;
            break;
        case kParamCH2PWM3:
            value = 0;
            break;
        case kParamCH2PWM4:
            value = 0;
            break;
        case kParamCH3Assign1:
            value = 255;
            break;
        case kParamCH3Assign2:
            value = 255;
            break;
        case kParamCH3Assign3:
            value = 255;
            break;
        case kParamCH3Assign4:
            value = 255;
            break;
        case kParamCH3PWM1:
            value = 0;
            break;
        case kParamCH3PWM2:
            value = 0;
            break;
        case kParamCH3PWM3:
            value = 0;
            break;
        case kParamCH3PWM4:
            value = 0;
            break;
        case kParamCH4Assign1:
            value = 255;
            break;
        case kParamCH4Assign2:
            value = 255;
            break;
        case kParamCH4Assign3:
            value = 255;
            break;
        case kParamCH4Assign4:
            value = 255;
            break;
        case kParamCH4PWM1:
            value = 0;
            break;
        case kParamCH4PWM2:
            value = 0;
            break;
        case kParamCH4PWM3:
            value = 0;
            break;
        case kParamCH4PWM4:
            value = 0;
            break;
        case kParamCH5Assign1:
            value = 255;
            break;
        case kParamCH5Assign2:
            value = 255;
            break;
        case kParamCH5Assign3:
            value = 255;
            break;
        case kParamCH5Assign4:
            value = 255;
            break;
        case kParamCH5PWM1:
            value = 0;
            break;
        case kParamCH5PWM2:
            value = 0;
            break;
        case kParamCH5PWM3:
            value = 0;
            break;
        case kParamCH5PWM4:
            value = 0;
            break;
    default:
        return;
    }
    param.set(value);
}

static const PROGMEM char _param_name_CH1Status[] = "CH1Status";
static const PROGMEM char _param_name_CH2Status[] = "CH2Status";
static const PROGMEM char _param_name_CH3Status[] = "CH3Status";
static const PROGMEM char _param_name_CH4Status[] = "CH4Status";
static const PROGMEM char _param_name_CH5Status[] = "CH5Status";
static const PROGMEM char _param_name_CH1Type[] = "CH1Type";
static const PROGMEM char _param_name_CH2Type[] = "CH2Type";
static const PROGMEM char _param_name_CH3Type[] = "CH3Type";
static const PROGMEM char _param_name_CH4Type[] = "CH4Type";
static const PROGMEM char _param_name_CH5Type[] = "CH5Type";
static const PROGMEM char _param_name_CH1Assign1[] = "CH1Assign1";
static const PROGMEM char _param_name_CH1Assign2[] = "CH1Assign2";
static const PROGMEM char _param_name_CH1Assign3[] = "CH1Assign3";
static const PROGMEM char _param_name_CH1Assign4[] = "CH1Assign4";
static const PROGMEM char _param_name_CH1PWM1[] = "CH1PWM1";
static const PROGMEM char _param_name_CH1PWM2[] = "CH1PWM2";
static const PROGMEM char _param_name_CH1PWM3[] = "CH1PWM3";
static const PROGMEM char _param_name_CH1PWM4[] = "CH1PWM4";
static const PROGMEM char _param_name_CH2Assign1[] = "CH2Assign1";
static const PROGMEM char _param_name_CH2Assign2[] = "CH2Assign2";
static const PROGMEM char _param_name_CH2Assign3[] = "CH2Assign3";
static const PROGMEM char _param_name_CH2Assign4[] = "CH2Assign4";
static const PROGMEM char _param_name_CH2PWM1[] = "CH2PWM1";
static const PROGMEM char _param_name_CH2PWM2[] = "CH2PWM2";
static const PROGMEM char _param_name_CH2PWM3[] = "CH2PWM3";
static const PROGMEM char _param_name_CH2PWM4[] = "CH2PWM4";
static const PROGMEM char _param_name_CH3Assign1[] = "CH3Assign1";
static const PROGMEM char _param_name_CH3Assign2[] = "CH3Assign2";
static const PROGMEM char _param_name_CH3Assign3[] = "CH3Assign3";
static const PROGMEM char _param_name_CH3Assign4[] = "CH3Assign4";
static const PROGMEM char _param_name_CH3PWM1[] = "CH3PWM1";
static const PROGMEM char _param_name_CH3PWM2[] = "CH3PWM2";
static const PROGMEM char _param_name_CH3PWM3[] = "CH3PWM3";
static const PROGMEM char _param_name_CH3PWM4[] = "CH3PWM4";
static const PROGMEM char _param_name_CH4Assign1[] = "CH4Assign1";
static const PROGMEM char _param_name_CH4Assign2[] = "CH4Assign2";
static const PROGMEM char _param_name_CH4Assign3[] = "CH4Assign3";
static const PROGMEM char _param_name_CH4Assign4[] = "CH4Assign4";
static const PROGMEM char _param_name_CH4PWM1[] = "CH4PWM1";
static const PROGMEM char _param_name_CH4PWM2[] = "CH4PWM2";
static const PROGMEM char _param_name_CH4PWM3[] = "CH4PWM3";
static const PROGMEM char _param_name_CH4PWM4[] = "CH4PWM4";
static const PROGMEM char _param_name_CH5Assign1[] = "CH5Assign1";
static const PROGMEM char _param_name_CH5Assign2[] = "CH5Assign2";
static const PROGMEM char _param_name_CH5Assign3[] = "CH5Assign3";
static const PROGMEM char _param_name_CH5Assign4[] = "CH5Assign4";
static const PROGMEM char _param_name_CH5PWM1[] = "CH5PWM1";
static const PROGMEM char _param_name_CH5PWM2[] = "CH5PWM2";
static const PROGMEM char _param_name_CH5PWM3[] = "CH5PWM3";
static const PROGMEM char _param_name_CH5PWM4[] = "CH5PWM4";

const PROGMEM char *
param_name(const Parameter &param)
{
    switch (param.address()) {
    case kParamCH1Status:
        return &_param_name_CH1Status[0];
    case kParamCH2Status:
        return &_param_name_CH2Status[0];
    case kParamCH3Status:
        return &_param_name_CH3Status[0];
    case kParamCH4Status:
        return &_param_name_CH4Status[0];
    case kParamCH5Status:
        return &_param_name_CH5Status[0];
    case kParamCH1Type:
        return &_param_name_CH1Type[0];
    case kParamCH2Type:
        return &_param_name_CH2Type[0];
    case kParamCH3Type:
        return &_param_name_CH3Type[0];
    case kParamCH4Type:
        return &_param_name_CH4Type[0];
    case kParamCH5Type:
        return &_param_name_CH5Type[0];
    case kParamCH1Assign1:
        return &_param_name_CH1Assign1[0];
    case kParamCH1Assign2:
        return &_param_name_CH1Assign2[0];
    case kParamCH1Assign3:
        return &_param_name_CH1Assign3[0];
    case kParamCH1Assign4:
        return &_param_name_CH1Assign4[0];
    case kParamCH1PWM1:
        return &_param_name_CH1PWM1[0];
    case kParamCH1PWM2:
        return &_param_name_CH1PWM2[0];
    case kParamCH1PWM3:
        return &_param_name_CH1PWM3[0];
    case kParamCH1PWM4:
        return &_param_name_CH1PWM4[0];
    case kParamCH2Assign1:
        return &_param_name_CH2Assign1[0];
    case kParamCH2Assign2:
        return &_param_name_CH2Assign2[0];
    case kParamCH2Assign3:
        return &_param_name_CH2Assign3[0];
    case kParamCH2Assign4:
        return &_param_name_CH2Assign4[0];
    case kParamCH2PWM1:
        return &_param_name_CH2PWM1[0];
    case kParamCH2PWM2:
        return &_param_name_CH2PWM2[0];
    case kParamCH2PWM3:
        return &_param_name_CH2PWM3[0];
    case kParamCH2PWM4:
        return &_param_name_CH2PWM4[0];
    case kParamCH3Assign1:
        return &_param_name_CH3Assign1[0];
    case kParamCH3Assign2:
        return &_param_name_CH3Assign2[0];
    case kParamCH3Assign3:
        return &_param_name_CH3Assign3[0];
    case kParamCH3Assign4:
        return &_param_name_CH3Assign4[0];
    case kParamCH3PWM1:
        return &_param_name_CH3PWM1[0];
    case kParamCH3PWM2:
        return &_param_name_CH3PWM2[0];
    case kParamCH3PWM3:
        return &_param_name_CH3PWM3[0];
    case kParamCH3PWM4:
        return &_param_name_CH3PWM4[0];
    case kParamCH4Assign1:
        return &_param_name_CH4Assign1[0];
    case kParamCH4Assign2:
        return &_param_name_CH4Assign2[0];
    case kParamCH4Assign3:
        return &_param_name_CH4Assign3[0];
    case kParamCH4Assign4:
        return &_param_name_CH4Assign4[0];
    case kParamCH4PWM1:
        return &_param_name_CH4PWM1[0];
    case kParamCH4PWM2:
        return &_param_name_CH4PWM2[0];
    case kParamCH4PWM3:
        return &_param_name_CH4PWM3[0];
    case kParamCH4PWM4:
        return &_param_name_CH4PWM4[0];
    case kParamCH5Assign1:
        return &_param_name_CH5Assign1[0];
    case kParamCH5Assign2:
        return &_param_name_CH5Assign2[0];
    case kParamCH5Assign3:
        return &_param_name_CH5Assign3[0];
    case kParamCH5Assign4:
        return &_param_name_CH5Assign4[0];
    case kParamCH5PWM1:
        return &_param_name_CH5PWM1[0];
    case kParamCH5PWM2:
        return &_param_name_CH5PWM2[0];
    case kParamCH5PWM3:
        return &_param_name_CH5PWM3[0];
    case kParamCH5PWM4:
        return &_param_name_CH5PWM4[0];
    }
    return nullptr;
}


} // namespace PowerV3
