// AUTOMATICALLY GENERATED - DO NOT EDIT
#include "lin_defs.h"


namespace Encoding
{
bool
invalid(uint8_t encoding, uint16_t value)
{
    if (encoding == kEncoding_board_function) {
        if ((value >= 0) && (value <= 6)) {
            return false;
        }
        if (value == 255) {
            return false;
        }
    }
    if (encoding == kEncoding_bootloader_magic) {
        if (value == 0) {
            return false;
        }
        if (value == 1) {
            return false;
        }
        if (value == 0x4f42) {
            return false;
        }
    }
    if (encoding == kEncoding_bl_status) {
        if ((value >= 0) && (value <= 4)) {
            return false;
        }
    }
    if (encoding == kEncoding_bl_reason) {
        if ((value >= 0) && (value <= 2)) {
            return false;
        }
    }
    if (encoding == kEncoding_input_assignment) {
        if ((value >= 0) && (value <= 23)) {
            return false;
        }
    }
    if (encoding == kEncoding_sp_input_mode) {
        if ((value >= 0) && (value <= 4)) {
            return false;
        }
    }
    if (encoding == kEncoding_sg_input_mode) {
        if ((value >= 2) && (value <= 4)) {
            return false;
        }
        if (value == 0) {
            return false;
        }
    }
    if (encoding == kEncoding_msec) {
        if ((value >= 100) && (value <= 65000)) {
            return false;
        }
        if (value == 0) {
            return false;
        }
    }
    if (encoding == kEncoding_up_to_10) {
        if ((value >= 1) && (value <= 10)) {
            return false;
        }
        if (value == 0) {
            return false;
        }
    }
    if (encoding == kEncoding_sec) {
        if ((value >= 1) && (value <= 600)) {
            return false;
        }
        if (value == 0) {
            return false;
        }
    }
    if (encoding == kEncoding_v1_output_status) {
        if (value == 0) {
            return false;
        }
        if (value == 1) {
            return false;
        }
    }
    if (encoding == kEncoding_v1_output_assignment) {
        if ((value >= 0) && (value <= 23)) {
            return false;
        }
    }
    if (encoding == kEncoding_v3_output_status) {
        if ((value >= 0) && (value <= 5)) {
            return false;
        }
    }
    if (encoding == kEncoding_v3_output_type) {
        if ((value >= 0) && (value <= 6)) {
            return false;
        }
    }
    if (encoding == kEncoding_v3_output_assignment) {
        if ((value >= 0) && (value <= 23)) {
            return false;
        }
    }
    if (encoding == kEncoding_pwm_duty_cycle) {
        if ((value >= 0) && (value <= 100)) {
            return false;
        }
    }
    return true;
}

static const PROGMEM char _encoding_name_board_function[] = "board_function";
static const PROGMEM char _encoding_info_board_function_0[] = "Unknown";
static const PROGMEM char _encoding_info_board_function_1[] = "Master";
static const PROGMEM char _encoding_info_board_function_2[] = "PowerV1";
static const PROGMEM char _encoding_info_board_function_3[] = "PowerV3";
static const PROGMEM char _encoding_info_board_function_4[] = "ECUBridge";
static const PROGMEM char _encoding_info_board_function_5[] = "Dashboard";
static const PROGMEM char _encoding_info_board_function_6[] = "Tester";
static const PROGMEM char _encoding_info_board_function_7[] = "Unconfigured";
static const PROGMEM char _encoding_name_bootloader_magic[] = "bootloader_magic";
static const PROGMEM char _encoding_info_bootloader_magic_0[] = "Program";
static const PROGMEM char _encoding_info_bootloader_magic_1[] = "Bootloader";
static const PROGMEM char _encoding_info_bootloader_magic_2[] = "EnterBootloader";
static const PROGMEM char _encoding_name_service_error[] = "service_error";
static const PROGMEM char _encoding_info_service_error_0[] = "FunctionNotSupported";
static const PROGMEM char _encoding_info_service_error_1[] = "IncorrectLength";
static const PROGMEM char _encoding_info_service_error_2[] = "ConditionsNotCorrect";
static const PROGMEM char _encoding_info_service_error_3[] = "OutOfRange";
static const PROGMEM char _encoding_info_service_error_4[] = "AccessDenied";
static const PROGMEM char _encoding_info_service_error_5[] = "GeneralFailure";
static const PROGMEM char _encoding_name_bl_status[] = "bl_status";
static const PROGMEM char _encoding_info_bl_status_0[] = "WaitingForProgrammer";
static const PROGMEM char _encoding_info_bl_status_1[] = "ReadyForPage";
static const PROGMEM char _encoding_info_bl_status_2[] = "PageInProgress";
static const PROGMEM char _encoding_info_bl_status_3[] = "PageCRCError";
static const PROGMEM char _encoding_info_bl_status_4[] = "PageAddressError";
static const PROGMEM char _encoding_name_bl_reason[] = "bl_reason";
static const PROGMEM char _encoding_info_bl_reason_0[] = "Unspecified";
static const PROGMEM char _encoding_info_bl_reason_1[] = "CRCMismatch";
static const PROGMEM char _encoding_info_bl_reason_2[] = "Forced";
static const PROGMEM char _encoding_name_input_assignment[] = "input_assignment";
static const PROGMEM char _encoding_info_input_assignment_0[] = "Unassigned";
static const PROGMEM char _encoding_info_input_assignment_1[] = "Ignition";
static const PROGMEM char _encoding_info_input_assignment_2[] = "Start";
static const PROGMEM char _encoding_info_input_assignment_3[] = "MarkerLights";
static const PROGMEM char _encoding_info_input_assignment_4[] = "HeadLights";
static const PROGMEM char _encoding_info_input_assignment_5[] = "HighBeam";
static const PROGMEM char _encoding_info_input_assignment_6[] = "HighBeamToggle";
static const PROGMEM char _encoding_info_input_assignment_7[] = "FogLight";
static const PROGMEM char _encoding_info_input_assignment_8[] = "LeftTurn";
static const PROGMEM char _encoding_info_input_assignment_9[] = "RightTurn";
static const PROGMEM char _encoding_info_input_assignment_10[] = "Brake";
static const PROGMEM char _encoding_info_input_assignment_11[] = "Reverse";
static const PROGMEM char _encoding_info_input_assignment_12[] = "Door";
static const PROGMEM char _encoding_info_input_assignment_13[] = "InteriorLight";
static const PROGMEM char _encoding_info_input_assignment_14[] = "Hazard";
static const PROGMEM char _encoding_info_input_assignment_15[] = "DoorUnlock";
static const PROGMEM char _encoding_info_input_assignment_16[] = "LightsUp";
static const PROGMEM char _encoding_info_input_assignment_17[] = "CabinFan1";
static const PROGMEM char _encoding_info_input_assignment_18[] = "CabinFan2";
static const PROGMEM char _encoding_info_input_assignment_19[] = "CabinFan3";
static const PROGMEM char _encoding_info_input_assignment_20[] = "WiperInt";
static const PROGMEM char _encoding_info_input_assignment_21[] = "WiperLow";
static const PROGMEM char _encoding_info_input_assignment_22[] = "WiperHigh";
static const PROGMEM char _encoding_info_input_assignment_23[] = "RearDefrost";
static const PROGMEM char _encoding_name_sp_input_mode[] = "sp_input_mode";
static const PROGMEM char _encoding_info_sp_input_mode_0[] = "ActiveLow";
static const PROGMEM char _encoding_info_sp_input_mode_1[] = "ActiveHigh";
static const PROGMEM char _encoding_info_sp_input_mode_2[] = "2mASense";
static const PROGMEM char _encoding_info_sp_input_mode_3[] = "16mASense";
static const PROGMEM char _encoding_info_sp_input_mode_4[] = "5VSense";
static const PROGMEM char _encoding_name_sg_input_mode[] = "sg_input_mode";
static const PROGMEM char _encoding_info_sg_input_mode_0[] = "ActiveLow";
static const PROGMEM char _encoding_info_sg_input_mode_1[] = "2mASense";
static const PROGMEM char _encoding_info_sg_input_mode_2[] = "16mASense";
static const PROGMEM char _encoding_info_sg_input_mode_3[] = "5VSense";
static const PROGMEM char _encoding_name_msec[] = "msec";
static const PROGMEM char _encoding_info_msec_0[] = "Disabled";
static const PROGMEM char _encoding_name_sec[] = "sec";
static const PROGMEM char _encoding_info_sec_0[] = "Disabled";
static const PROGMEM char _encoding_name_up_to_10[] = "up_to_10";
static const PROGMEM char _encoding_info_up_to_10_0[] = "Disabled";
static const PROGMEM char _encoding_name_node_address[] = "node_address";
static const PROGMEM char _encoding_info_node_address_0[] = "Sleep";
static const PROGMEM char _encoding_info_node_address_2[] = "Functional";
static const PROGMEM char _encoding_info_node_address_3[] = "Broadcast";
static const PROGMEM char _encoding_name_pci[] = "pci";
static const PROGMEM char _encoding_info_pci_0[] = "SingleFrame";
static const PROGMEM char _encoding_info_pci_1[] = "FirstFrame";
static const PROGMEM char _encoding_info_pci_2[] = "ContinuationFrame";
static const PROGMEM char _encoding_name_service_id[] = "service_id";
static const PROGMEM char _encoding_info_service_id_0[] = "ReadDataByID";
static const PROGMEM char _encoding_info_service_id_1[] = "WriteDataByID";
static const PROGMEM char _encoding_info_service_id_2[] = "TesterPresent";
static const PROGMEM char _encoding_info_service_id_3[] = "ReadByID";
static const PROGMEM char _encoding_info_service_id_4[] = "DataDump";
static const PROGMEM char _encoding_info_service_id_5[] = "ErrorResponse";
static const PROGMEM char _encoding_info_service_id_6[] = "ResponseOffset";
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
static const PROGMEM char _encoding_name_v3_output_type[] = "v3_output_type";
static const PROGMEM char _encoding_info_v3_output_type_0[] = "5AGeneric";
static const PROGMEM char _encoding_info_v3_output_type_1[] = "10AGeneric";
static const PROGMEM char _encoding_info_v3_output_type_2[] = "LED";
static const PROGMEM char _encoding_info_v3_output_type_3[] = "HID";
static const PROGMEM char _encoding_info_v3_output_type_4[] = "LowPowerBulb";
static const PROGMEM char _encoding_info_v3_output_type_5[] = "HighPowerBulb";
static const PROGMEM char _encoding_info_v3_output_type_6[] = "Motor";
static const PROGMEM char _encoding_name_v3_output_assignment[] = "v3_output_assignment";
static const PROGMEM char _encoding_info_v3_output_assignment_0[] = "Unassigned";
static const PROGMEM char _encoding_info_v3_output_assignment_1[] = "Ignition";
static const PROGMEM char _encoding_info_v3_output_assignment_2[] = "Start";
static const PROGMEM char _encoding_info_v3_output_assignment_3[] = "LightsUp";
static const PROGMEM char _encoding_info_v3_output_assignment_4[] = "LightsDown";
static const PROGMEM char _encoding_info_v3_output_assignment_5[] = "HeadLights";
static const PROGMEM char _encoding_info_v3_output_assignment_6[] = "LowBeam";
static const PROGMEM char _encoding_info_v3_output_assignment_7[] = "HighBeam";
static const PROGMEM char _encoding_info_v3_output_assignment_8[] = "FogLights";
static const PROGMEM char _encoding_info_v3_output_assignment_9[] = "MarkerLights";
static const PROGMEM char _encoding_info_v3_output_assignment_10[] = "LeftTurn";
static const PROGMEM char _encoding_info_v3_output_assignment_11[] = "LeftTurnMarker";
static const PROGMEM char _encoding_info_v3_output_assignment_12[] = "RightTurn";
static const PROGMEM char _encoding_info_v3_output_assignment_13[] = "RightTurnMarker";
static const PROGMEM char _encoding_info_v3_output_assignment_14[] = "Brake";
static const PROGMEM char _encoding_info_v3_output_assignment_15[] = "Reverse";
static const PROGMEM char _encoding_info_v3_output_assignment_16[] = "InteriorLight";
static const PROGMEM char _encoding_info_v3_output_assignment_17[] = "CabinFan1";
static const PROGMEM char _encoding_info_v3_output_assignment_18[] = "CabinFan2";
static const PROGMEM char _encoding_info_v3_output_assignment_19[] = "CabinFan3";
static const PROGMEM char _encoding_info_v3_output_assignment_20[] = "CabinFan4";
static const PROGMEM char _encoding_info_v3_output_assignment_21[] = "WiperLow";
static const PROGMEM char _encoding_info_v3_output_assignment_22[] = "WiperHigh";
static const PROGMEM char _encoding_info_v3_output_assignment_23[] = "RearDefrost";
static const PROGMEM char _encoding_name_pwm_duty_cycle[] = "pwm_duty_cycle";
static const PROGMEM char _encoding_name_RPM[] = "RPM";
static const PROGMEM char _encoding_name_PSI[] = "PSI";
static const PROGMEM char _encoding_name_F[] = "F";
static const PROGMEM char _encoding_name_V[] = "V";
static const PROGMEM char _encoding_name_lambda[] = "lambda";
static const PROGMEM char _encoding_name_MPH[] = "MPH";

const PROGMEM char *
name(uint8_t encoding)
{
    if (encoding == kEncoding_board_function) {
        return &_encoding_name_board_function[0];
    }
    if (encoding == kEncoding_bootloader_magic) {
        return &_encoding_name_bootloader_magic[0];
    }
    if (encoding == kEncoding_service_error) {
        return &_encoding_name_service_error[0];
    }
    if (encoding == kEncoding_bl_status) {
        return &_encoding_name_bl_status[0];
    }
    if (encoding == kEncoding_bl_reason) {
        return &_encoding_name_bl_reason[0];
    }
    if (encoding == kEncoding_input_assignment) {
        return &_encoding_name_input_assignment[0];
    }
    if (encoding == kEncoding_sp_input_mode) {
        return &_encoding_name_sp_input_mode[0];
    }
    if (encoding == kEncoding_sg_input_mode) {
        return &_encoding_name_sg_input_mode[0];
    }
    if (encoding == kEncoding_msec) {
        return &_encoding_name_msec[0];
    }
    if (encoding == kEncoding_sec) {
        return &_encoding_name_sec[0];
    }
    if (encoding == kEncoding_up_to_10) {
        return &_encoding_name_up_to_10[0];
    }
    if (encoding == kEncoding_node_address) {
        return &_encoding_name_node_address[0];
    }
    if (encoding == kEncoding_pci) {
        return &_encoding_name_pci[0];
    }
    if (encoding == kEncoding_service_id) {
        return &_encoding_name_service_id[0];
    }
    if (encoding == kEncoding_v1_output_status) {
        return &_encoding_name_v1_output_status[0];
    }
    if (encoding == kEncoding_v1_output_assignment) {
        return &_encoding_name_v1_output_assignment[0];
    }
    if (encoding == kEncoding_v3_output_status) {
        return &_encoding_name_v3_output_status[0];
    }
    if (encoding == kEncoding_v3_output_type) {
        return &_encoding_name_v3_output_type[0];
    }
    if (encoding == kEncoding_v3_output_assignment) {
        return &_encoding_name_v3_output_assignment[0];
    }
    if (encoding == kEncoding_pwm_duty_cycle) {
        return &_encoding_name_pwm_duty_cycle[0];
    }
    if (encoding == kEncoding_RPM) {
        return &_encoding_name_RPM[0];
    }
    if (encoding == kEncoding_PSI) {
        return &_encoding_name_PSI[0];
    }
    if (encoding == kEncoding_F) {
        return &_encoding_name_F[0];
    }
    if (encoding == kEncoding_V) {
        return &_encoding_name_V[0];
    }
    if (encoding == kEncoding_lambda) {
        return &_encoding_name_lambda[0];
    }
    if (encoding == kEncoding_MPH) {
        return &_encoding_name_MPH[0];
    }
    return nullptr;
}

const PROGMEM char *
info(uint8_t encoding, uint16_t value)
{
    if (encoding == kEncoding_board_function) {
        if (value == 0) {
            return &_encoding_info_board_function_0[0];
        }
        if (value == 1) {
            return &_encoding_info_board_function_1[0];
        }
        if (value == 2) {
            return &_encoding_info_board_function_2[0];
        }
        if (value == 3) {
            return &_encoding_info_board_function_3[0];
        }
        if (value == 4) {
            return &_encoding_info_board_function_4[0];
        }
        if (value == 5) {
            return &_encoding_info_board_function_5[0];
        }
        if (value == 6) {
            return &_encoding_info_board_function_6[0];
        }
        if (value == 255) {
            return &_encoding_info_board_function_7[0];
        }
    }
    if (encoding == kEncoding_bootloader_magic) {
        if (value == 0) {
            return &_encoding_info_bootloader_magic_0[0];
        }
        if (value == 1) {
            return &_encoding_info_bootloader_magic_1[0];
        }
        if (value == 0x4f42) {
            return &_encoding_info_bootloader_magic_2[0];
        }
    }
    if (encoding == kEncoding_service_error) {
        if (value == 0x12) {
            return &_encoding_info_service_error_0[0];
        }
        if (value == 0x13) {
            return &_encoding_info_service_error_1[0];
        }
        if (value == 0x22) {
            return &_encoding_info_service_error_2[0];
        }
        if (value == 0x31) {
            return &_encoding_info_service_error_3[0];
        }
        if (value == 0x33) {
            return &_encoding_info_service_error_4[0];
        }
        if (value == 0x72) {
            return &_encoding_info_service_error_5[0];
        }
    }
    if (encoding == kEncoding_bl_status) {
        if (value == 0) {
            return &_encoding_info_bl_status_0[0];
        }
        if (value == 1) {
            return &_encoding_info_bl_status_1[0];
        }
        if (value == 2) {
            return &_encoding_info_bl_status_2[0];
        }
        if (value == 3) {
            return &_encoding_info_bl_status_3[0];
        }
        if (value == 4) {
            return &_encoding_info_bl_status_4[0];
        }
    }
    if (encoding == kEncoding_bl_reason) {
        if (value == 0) {
            return &_encoding_info_bl_reason_0[0];
        }
        if (value == 1) {
            return &_encoding_info_bl_reason_1[0];
        }
        if (value == 2) {
            return &_encoding_info_bl_reason_2[0];
        }
    }
    if (encoding == kEncoding_input_assignment) {
        if (value == 0) {
            return &_encoding_info_input_assignment_0[0];
        }
        if (value == 1) {
            return &_encoding_info_input_assignment_1[0];
        }
        if (value == 2) {
            return &_encoding_info_input_assignment_2[0];
        }
        if (value == 3) {
            return &_encoding_info_input_assignment_3[0];
        }
        if (value == 4) {
            return &_encoding_info_input_assignment_4[0];
        }
        if (value == 5) {
            return &_encoding_info_input_assignment_5[0];
        }
        if (value == 6) {
            return &_encoding_info_input_assignment_6[0];
        }
        if (value == 7) {
            return &_encoding_info_input_assignment_7[0];
        }
        if (value == 8) {
            return &_encoding_info_input_assignment_8[0];
        }
        if (value == 9) {
            return &_encoding_info_input_assignment_9[0];
        }
        if (value == 10) {
            return &_encoding_info_input_assignment_10[0];
        }
        if (value == 11) {
            return &_encoding_info_input_assignment_11[0];
        }
        if (value == 12) {
            return &_encoding_info_input_assignment_12[0];
        }
        if (value == 13) {
            return &_encoding_info_input_assignment_13[0];
        }
        if (value == 14) {
            return &_encoding_info_input_assignment_14[0];
        }
        if (value == 15) {
            return &_encoding_info_input_assignment_15[0];
        }
        if (value == 16) {
            return &_encoding_info_input_assignment_16[0];
        }
        if (value == 17) {
            return &_encoding_info_input_assignment_17[0];
        }
        if (value == 18) {
            return &_encoding_info_input_assignment_18[0];
        }
        if (value == 19) {
            return &_encoding_info_input_assignment_19[0];
        }
        if (value == 20) {
            return &_encoding_info_input_assignment_20[0];
        }
        if (value == 21) {
            return &_encoding_info_input_assignment_21[0];
        }
        if (value == 22) {
            return &_encoding_info_input_assignment_22[0];
        }
        if (value == 23) {
            return &_encoding_info_input_assignment_23[0];
        }
    }
    if (encoding == kEncoding_sp_input_mode) {
        if (value == 0) {
            return &_encoding_info_sp_input_mode_0[0];
        }
        if (value == 1) {
            return &_encoding_info_sp_input_mode_1[0];
        }
        if (value == 2) {
            return &_encoding_info_sp_input_mode_2[0];
        }
        if (value == 3) {
            return &_encoding_info_sp_input_mode_3[0];
        }
        if (value == 4) {
            return &_encoding_info_sp_input_mode_4[0];
        }
    }
    if (encoding == kEncoding_sg_input_mode) {
        if (value == 0) {
            return &_encoding_info_sg_input_mode_0[0];
        }
        if (value == 2) {
            return &_encoding_info_sg_input_mode_1[0];
        }
        if (value == 3) {
            return &_encoding_info_sg_input_mode_2[0];
        }
        if (value == 4) {
            return &_encoding_info_sg_input_mode_3[0];
        }
    }
    if (encoding == kEncoding_msec) {
        if (value == 0) {
            return &_encoding_info_msec_0[0];
        }
    }
    if (encoding == kEncoding_sec) {
        if (value == 0) {
            return &_encoding_info_sec_0[0];
        }
    }
    if (encoding == kEncoding_up_to_10) {
        if (value == 0) {
            return &_encoding_info_up_to_10_0[0];
        }
    }
    if (encoding == kEncoding_node_address) {
        if (value == 0) {
            return &_encoding_info_node_address_0[0];
        }
        if (value == 126) {
            return &_encoding_info_node_address_2[0];
        }
        if (value == 127) {
            return &_encoding_info_node_address_3[0];
        }
    }
    if (encoding == kEncoding_pci) {
        if (value == 0) {
            return &_encoding_info_pci_0[0];
        }
        if (value == 1) {
            return &_encoding_info_pci_1[0];
        }
        if (value == 2) {
            return &_encoding_info_pci_2[0];
        }
    }
    if (encoding == kEncoding_service_id) {
        if (value == 0x22) {
            return &_encoding_info_service_id_0[0];
        }
        if (value == 0x2e) {
            return &_encoding_info_service_id_1[0];
        }
        if (value == 0x3e) {
            return &_encoding_info_service_id_2[0];
        }
        if (value == 0xb2) {
            return &_encoding_info_service_id_3[0];
        }
        if (value == 0xb4) {
            return &_encoding_info_service_id_4[0];
        }
        if (value == 0x7f) {
            return &_encoding_info_service_id_5[0];
        }
        if (value == 0x40) {
            return &_encoding_info_service_id_6[0];
        }
    }
    if (encoding == kEncoding_v1_output_status) {
        if (value == 0) {
            return &_encoding_info_v1_output_status_0[0];
        }
        if (value == 1) {
            return &_encoding_info_v1_output_status_1[0];
        }
    }
    if (encoding == kEncoding_v1_output_assignment) {
        if (value == 0) {
            return &_encoding_info_v1_output_assignment_0[0];
        }
        if (value == 1) {
            return &_encoding_info_v1_output_assignment_1[0];
        }
        if (value == 2) {
            return &_encoding_info_v1_output_assignment_2[0];
        }
        if (value == 3) {
            return &_encoding_info_v1_output_assignment_3[0];
        }
        if (value == 4) {
            return &_encoding_info_v1_output_assignment_4[0];
        }
        if (value == 5) {
            return &_encoding_info_v1_output_assignment_5[0];
        }
        if (value == 6) {
            return &_encoding_info_v1_output_assignment_6[0];
        }
        if (value == 7) {
            return &_encoding_info_v1_output_assignment_7[0];
        }
        if (value == 8) {
            return &_encoding_info_v1_output_assignment_8[0];
        }
        if (value == 9) {
            return &_encoding_info_v1_output_assignment_9[0];
        }
        if (value == 10) {
            return &_encoding_info_v1_output_assignment_10[0];
        }
        if (value == 11) {
            return &_encoding_info_v1_output_assignment_11[0];
        }
        if (value == 12) {
            return &_encoding_info_v1_output_assignment_12[0];
        }
        if (value == 13) {
            return &_encoding_info_v1_output_assignment_13[0];
        }
        if (value == 14) {
            return &_encoding_info_v1_output_assignment_14[0];
        }
        if (value == 15) {
            return &_encoding_info_v1_output_assignment_15[0];
        }
        if (value == 16) {
            return &_encoding_info_v1_output_assignment_16[0];
        }
        if (value == 17) {
            return &_encoding_info_v1_output_assignment_17[0];
        }
        if (value == 18) {
            return &_encoding_info_v1_output_assignment_18[0];
        }
        if (value == 19) {
            return &_encoding_info_v1_output_assignment_19[0];
        }
        if (value == 20) {
            return &_encoding_info_v1_output_assignment_20[0];
        }
        if (value == 21) {
            return &_encoding_info_v1_output_assignment_21[0];
        }
        if (value == 22) {
            return &_encoding_info_v1_output_assignment_22[0];
        }
        if (value == 23) {
            return &_encoding_info_v1_output_assignment_23[0];
        }
    }
    if (encoding == kEncoding_v3_output_status) {
        if (value == 0) {
            return &_encoding_info_v3_output_status_0[0];
        }
        if (value == 1) {
            return &_encoding_info_v3_output_status_1[0];
        }
        if (value == 2) {
            return &_encoding_info_v3_output_status_2[0];
        }
        if (value == 3) {
            return &_encoding_info_v3_output_status_3[0];
        }
        if (value == 4) {
            return &_encoding_info_v3_output_status_4[0];
        }
        if (value == 5) {
            return &_encoding_info_v3_output_status_5[0];
        }
    }
    if (encoding == kEncoding_v3_output_type) {
        if (value == 0) {
            return &_encoding_info_v3_output_type_0[0];
        }
        if (value == 1) {
            return &_encoding_info_v3_output_type_1[0];
        }
        if (value == 2) {
            return &_encoding_info_v3_output_type_2[0];
        }
        if (value == 3) {
            return &_encoding_info_v3_output_type_3[0];
        }
        if (value == 4) {
            return &_encoding_info_v3_output_type_4[0];
        }
        if (value == 5) {
            return &_encoding_info_v3_output_type_5[0];
        }
        if (value == 6) {
            return &_encoding_info_v3_output_type_6[0];
        }
    }
    if (encoding == kEncoding_v3_output_assignment) {
        if (value == 0) {
            return &_encoding_info_v3_output_assignment_0[0];
        }
        if (value == 1) {
            return &_encoding_info_v3_output_assignment_1[0];
        }
        if (value == 2) {
            return &_encoding_info_v3_output_assignment_2[0];
        }
        if (value == 3) {
            return &_encoding_info_v3_output_assignment_3[0];
        }
        if (value == 4) {
            return &_encoding_info_v3_output_assignment_4[0];
        }
        if (value == 5) {
            return &_encoding_info_v3_output_assignment_5[0];
        }
        if (value == 6) {
            return &_encoding_info_v3_output_assignment_6[0];
        }
        if (value == 7) {
            return &_encoding_info_v3_output_assignment_7[0];
        }
        if (value == 8) {
            return &_encoding_info_v3_output_assignment_8[0];
        }
        if (value == 9) {
            return &_encoding_info_v3_output_assignment_9[0];
        }
        if (value == 10) {
            return &_encoding_info_v3_output_assignment_10[0];
        }
        if (value == 11) {
            return &_encoding_info_v3_output_assignment_11[0];
        }
        if (value == 12) {
            return &_encoding_info_v3_output_assignment_12[0];
        }
        if (value == 13) {
            return &_encoding_info_v3_output_assignment_13[0];
        }
        if (value == 14) {
            return &_encoding_info_v3_output_assignment_14[0];
        }
        if (value == 15) {
            return &_encoding_info_v3_output_assignment_15[0];
        }
        if (value == 16) {
            return &_encoding_info_v3_output_assignment_16[0];
        }
        if (value == 17) {
            return &_encoding_info_v3_output_assignment_17[0];
        }
        if (value == 18) {
            return &_encoding_info_v3_output_assignment_18[0];
        }
        if (value == 19) {
            return &_encoding_info_v3_output_assignment_19[0];
        }
        if (value == 20) {
            return &_encoding_info_v3_output_assignment_20[0];
        }
        if (value == 21) {
            return &_encoding_info_v3_output_assignment_21[0];
        }
        if (value == 22) {
            return &_encoding_info_v3_output_assignment_22[0];
        }
        if (value == 23) {
            return &_encoding_info_v3_output_assignment_23[0];
        }
    }
    if (encoding == kEncoding_pwm_duty_cycle) {
    }
    if (encoding == kEncoding_RPM) {
    }
    if (encoding == kEncoding_PSI) {
    }
    if (encoding == kEncoding_F) {
    }
    if (encoding == kEncoding_V) {
    }
    if (encoding == kEncoding_lambda) {
    }
    if (encoding == kEncoding_MPH) {
    }
    return nullptr;
}
} // namespace Encoding

namespace Frame
{
static const PROGMEM char _frame_name_SlaveResponse[] = "SlaveResponse";
static const PROGMEM char _frame_name_Relays[] = "Relays";
static const PROGMEM char _frame_name_MasterRequest[] = "MasterRequest";
static const PROGMEM char _frame_name_ECUData[] = "ECUData";

const PROGMEM char *
name(uint8_t fid)
{
    if (fid == kFrameIDSlaveResponse) {
        return &_frame_name_SlaveResponse[0];
    }
    if (fid == kFrameIDRelays) {
        return &_frame_name_Relays[0];
    }
    if (fid == kFrameIDMasterRequest) {
        return &_frame_name_MasterRequest[0];
    }
    if (fid == kFrameIDECUData) {
        return &_frame_name_ECUData[0];
    }
    return nullptr;
}
} // namespace Frame

namespace Generic
{

bool
param_exists(Parameter::Address address)
{
    if (address == kParamProtocolVersion) {
        return true;
    }
    if (address == kParamBoardFunction) {
        return true;
    }
    if (address == kParamBootloaderMode) {
        return true;
    }
    if (address == kParamFirmwareVersion) {
        return true;
    }
    if (address == kParamFirmwarePageSize) {
        return true;
    }
    if (address == kParamWatchdogResets) {
        return true;
    }
    if (address == kParamSupplyVoltage) {
        return true;
    }
    if (address == kParamTemperature) {
        return true;
    }
    if (address == kParamLine) {
        return true;
    }
    if (address == kParamChecksum) {
        return true;
    }
    if (address == kParamParity) {
        return true;
    }
    if (address == kParamFraming) {
        return true;
    }
    if (address == kParamSynch) {
        return true;
    }
    if (address == kParamProtocol) {
        return true;
    }
    if (address == kParamConfigBase) {
        return true;
    }
    if (address == kParamConfigTop) {
        return true;
    }
    return false;
}

uint16_t
param_default(Parameter::Address address)
{
    return 0;
}

uint8_t
param_encoding(Parameter::Address address)
{
    if (address == kParamBoardFunction) {
        return kEncoding_board_function;
    }
    if (address == kParamBootloaderMode) {
        return kEncoding_bootloader_magic;
    }
    return kEncoding_none;
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
static const PROGMEM char _param_name_ConfigTop[] = "ConfigTop";

const PROGMEM char *
param_name(Parameter::Address address)
{
    if (address == kParamProtocolVersion) {
        return &_param_name_ProtocolVersion[0];
    }
    if (address == kParamBoardFunction) {
        return &_param_name_BoardFunction[0];
    }
    if (address == kParamBootloaderMode) {
        return &_param_name_BootloaderMode[0];
    }
    if (address == kParamFirmwareVersion) {
        return &_param_name_FirmwareVersion[0];
    }
    if (address == kParamFirmwarePageSize) {
        return &_param_name_FirmwarePageSize[0];
    }
    if (address == kParamWatchdogResets) {
        return &_param_name_WatchdogResets[0];
    }
    if (address == kParamSupplyVoltage) {
        return &_param_name_SupplyVoltage[0];
    }
    if (address == kParamTemperature) {
        return &_param_name_Temperature[0];
    }
    if (address == kParamLine) {
        return &_param_name_Line[0];
    }
    if (address == kParamChecksum) {
        return &_param_name_Checksum[0];
    }
    if (address == kParamParity) {
        return &_param_name_Parity[0];
    }
    if (address == kParamFraming) {
        return &_param_name_Framing[0];
    }
    if (address == kParamSynch) {
        return &_param_name_Synch[0];
    }
    if (address == kParamProtocol) {
        return &_param_name_Protocol[0];
    }
    if (address == kParamConfigBase) {
        return &_param_name_ConfigBase[0];
    }
    if (address == kParamConfigTop) {
        return &_param_name_ConfigTop[0];
    }
    return nullptr;
}


} // namespace Generic

namespace Bootloader
{

bool
param_exists(Parameter::Address address)
{
    if (address == kParamStatus) {
        return true;
    }
    if (address == kParamPageAddress) {
        return true;
    }
    if (address == kParamPageOffset) {
        return true;
    }
    if (address == kParamPageCRC) {
        return true;
    }
    if (address == kParamDebugPointer) {
        return true;
    }
    if (address == kParamReason) {
        return true;
    }
    if (address == kParamMemory) {
        return true;
    }
    if (address == kParamEEPROM) {
        return true;
    }
    return false;
}

uint16_t
param_default(Parameter::Address address)
{
    return 0;
}

uint8_t
param_encoding(Parameter::Address address)
{
    if (address == kParamStatus) {
        return kEncoding_bl_status;
    }
    if (address == kParamReason) {
        return kEncoding_bl_reason;
    }
    return kEncoding_none;
}

static const PROGMEM char _param_name_Status[] = "Status";
static const PROGMEM char _param_name_PageAddress[] = "PageAddress";
static const PROGMEM char _param_name_PageOffset[] = "PageOffset";
static const PROGMEM char _param_name_PageCRC[] = "PageCRC";
static const PROGMEM char _param_name_DebugPointer[] = "DebugPointer";
static const PROGMEM char _param_name_Reason[] = "Reason";
static const PROGMEM char _param_name_Memory[] = "Memory";
static const PROGMEM char _param_name_EEPROM[] = "EEPROM";

const PROGMEM char *
param_name(Parameter::Address address)
{
    if (address == kParamStatus) {
        return &_param_name_Status[0];
    }
    if (address == kParamPageAddress) {
        return &_param_name_PageAddress[0];
    }
    if (address == kParamPageOffset) {
        return &_param_name_PageOffset[0];
    }
    if (address == kParamPageCRC) {
        return &_param_name_PageCRC[0];
    }
    if (address == kParamDebugPointer) {
        return &_param_name_DebugPointer[0];
    }
    if (address == kParamReason) {
        return &_param_name_Reason[0];
    }
    if (address == kParamMemory) {
        return &_param_name_Memory[0];
    }
    if (address == kParamEEPROM) {
        return &_param_name_EEPROM[0];
    }
    return nullptr;
}


} // namespace Bootloader

namespace Master
{

bool
param_exists(Parameter::Address address)
{
    if (address == kParamSP1Assign) {
        return true;
    }
    if (address == kParamSP1Mode) {
        return true;
    }
    if (address == kParamSP2Assign) {
        return true;
    }
    if (address == kParamSP2Mode) {
        return true;
    }
    if (address == kParamSP3Assign) {
        return true;
    }
    if (address == kParamSP3Mode) {
        return true;
    }
    if (address == kParamSP4Assign) {
        return true;
    }
    if (address == kParamSP4Mode) {
        return true;
    }
    if (address == kParamSP5Assign) {
        return true;
    }
    if (address == kParamSP5Mode) {
        return true;
    }
    if (address == kParamSP6Assign) {
        return true;
    }
    if (address == kParamSP6Mode) {
        return true;
    }
    if (address == kParamSP7Assign) {
        return true;
    }
    if (address == kParamSP7Mode) {
        return true;
    }
    if (address == kParamSG0Assign) {
        return true;
    }
    if (address == kParamSG0Mode) {
        return true;
    }
    if (address == kParamSG1Assign) {
        return true;
    }
    if (address == kParamSG1Mode) {
        return true;
    }
    if (address == kParamSG2Assign) {
        return true;
    }
    if (address == kParamSG2Mode) {
        return true;
    }
    if (address == kParamSG3Assign) {
        return true;
    }
    if (address == kParamSG3Mode) {
        return true;
    }
    if (address == kParamSG4Assign) {
        return true;
    }
    if (address == kParamSG4Mode) {
        return true;
    }
    if (address == kParamSG5Assign) {
        return true;
    }
    if (address == kParamSG5Mode) {
        return true;
    }
    if (address == kParamSG6Assign) {
        return true;
    }
    if (address == kParamSG6Mode) {
        return true;
    }
    if (address == kParamSG7Assign) {
        return true;
    }
    if (address == kParamSG7Mode) {
        return true;
    }
    if (address == kParamSG8Assign) {
        return true;
    }
    if (address == kParamSG8Mode) {
        return true;
    }
    if (address == kParamSG9Assign) {
        return true;
    }
    if (address == kParamSG9Mode) {
        return true;
    }
    if (address == kParamSG10Assign) {
        return true;
    }
    if (address == kParamSG11Assign) {
        return true;
    }
    if (address == kParamSG12Assign) {
        return true;
    }
    if (address == kParamSG13Assign) {
        return true;
    }
    if (address == kParamTurnBlinkPeriod) {
        return true;
    }
    if (address == kParamPassingBlinkCount) {
        return true;
    }
    if (address == kParamPathLightTime) {
        return true;
    }
    if (address == kParamInteriorLightTime) {
        return true;
    }
    if (address == kParamWelcomeLightTime) {
        return true;
    }
    if (address == kParamBrakeBlinkPeriod) {
        return true;
    }
    if (address == kParamBrakeBlinkCount) {
        return true;
    }
    if (address == kParamWiperInterval) {
        return true;
    }
    return false;
}

uint16_t
param_default(Parameter::Address address)
{
    if (address == kParamSP1Assign) {
        return 0;
    }
    if (address == kParamSP1Mode) {
        return 1;
    }
    if (address == kParamSP2Assign) {
        return 0;
    }
    if (address == kParamSP2Mode) {
        return 1;
    }
    if (address == kParamSP3Assign) {
        return 0;
    }
    if (address == kParamSP3Mode) {
        return 1;
    }
    if (address == kParamSP4Assign) {
        return 0;
    }
    if (address == kParamSP4Mode) {
        return 1;
    }
    if (address == kParamSP5Assign) {
        return 0;
    }
    if (address == kParamSP5Mode) {
        return 1;
    }
    if (address == kParamSP6Assign) {
        return 0;
    }
    if (address == kParamSP6Mode) {
        return 1;
    }
    if (address == kParamSP7Assign) {
        return 0;
    }
    if (address == kParamSP7Mode) {
        return 1;
    }
    if (address == kParamSG0Assign) {
        return 0;
    }
    if (address == kParamSG0Mode) {
        return 0;
    }
    if (address == kParamSG1Assign) {
        return 0;
    }
    if (address == kParamSG1Mode) {
        return 0;
    }
    if (address == kParamSG2Assign) {
        return 0;
    }
    if (address == kParamSG2Mode) {
        return 0;
    }
    if (address == kParamSG3Assign) {
        return 0;
    }
    if (address == kParamSG3Mode) {
        return 0;
    }
    if (address == kParamSG4Assign) {
        return 0;
    }
    if (address == kParamSG4Mode) {
        return 0;
    }
    if (address == kParamSG5Assign) {
        return 0;
    }
    if (address == kParamSG5Mode) {
        return 0;
    }
    if (address == kParamSG6Assign) {
        return 0;
    }
    if (address == kParamSG6Mode) {
        return 0;
    }
    if (address == kParamSG7Assign) {
        return 0;
    }
    if (address == kParamSG7Mode) {
        return 0;
    }
    if (address == kParamSG8Assign) {
        return 0;
    }
    if (address == kParamSG8Mode) {
        return 0;
    }
    if (address == kParamSG9Assign) {
        return 0;
    }
    if (address == kParamSG9Mode) {
        return 0;
    }
    if (address == kParamSG10Assign) {
        return 0;
    }
    if (address == kParamSG11Assign) {
        return 0;
    }
    if (address == kParamSG12Assign) {
        return 0;
    }
    if (address == kParamSG13Assign) {
        return 0;
    }
    if (address == kParamTurnBlinkPeriod) {
        return 500;
    }
    if (address == kParamPassingBlinkCount) {
        return 0;
    }
    if (address == kParamPathLightTime) {
        return 30;
    }
    if (address == kParamInteriorLightTime) {
        return 30;
    }
    if (address == kParamWelcomeLightTime) {
        return 30;
    }
    if (address == kParamBrakeBlinkPeriod) {
        return 0;
    }
    if (address == kParamBrakeBlinkCount) {
        return 0;
    }
    if (address == kParamWiperInterval) {
        return 0;
    }
    return 0;
}

uint8_t
param_encoding(Parameter::Address address)
{
    if (address == kParamSP1Assign) {
        return kEncoding_input_assignment;
    }
    if (address == kParamSP1Mode) {
        return kEncoding_sp_input_mode;
    }
    if (address == kParamSP2Assign) {
        return kEncoding_input_assignment;
    }
    if (address == kParamSP2Mode) {
        return kEncoding_sp_input_mode;
    }
    if (address == kParamSP3Assign) {
        return kEncoding_input_assignment;
    }
    if (address == kParamSP3Mode) {
        return kEncoding_sp_input_mode;
    }
    if (address == kParamSP4Assign) {
        return kEncoding_input_assignment;
    }
    if (address == kParamSP4Mode) {
        return kEncoding_sp_input_mode;
    }
    if (address == kParamSP5Assign) {
        return kEncoding_input_assignment;
    }
    if (address == kParamSP5Mode) {
        return kEncoding_sp_input_mode;
    }
    if (address == kParamSP6Assign) {
        return kEncoding_input_assignment;
    }
    if (address == kParamSP6Mode) {
        return kEncoding_sp_input_mode;
    }
    if (address == kParamSP7Assign) {
        return kEncoding_input_assignment;
    }
    if (address == kParamSP7Mode) {
        return kEncoding_sp_input_mode;
    }
    if (address == kParamSG0Assign) {
        return kEncoding_input_assignment;
    }
    if (address == kParamSG0Mode) {
        return kEncoding_sg_input_mode;
    }
    if (address == kParamSG1Assign) {
        return kEncoding_input_assignment;
    }
    if (address == kParamSG1Mode) {
        return kEncoding_sg_input_mode;
    }
    if (address == kParamSG2Assign) {
        return kEncoding_input_assignment;
    }
    if (address == kParamSG2Mode) {
        return kEncoding_sg_input_mode;
    }
    if (address == kParamSG3Assign) {
        return kEncoding_input_assignment;
    }
    if (address == kParamSG3Mode) {
        return kEncoding_sg_input_mode;
    }
    if (address == kParamSG4Assign) {
        return kEncoding_input_assignment;
    }
    if (address == kParamSG4Mode) {
        return kEncoding_sg_input_mode;
    }
    if (address == kParamSG5Assign) {
        return kEncoding_input_assignment;
    }
    if (address == kParamSG5Mode) {
        return kEncoding_sg_input_mode;
    }
    if (address == kParamSG6Assign) {
        return kEncoding_input_assignment;
    }
    if (address == kParamSG6Mode) {
        return kEncoding_sg_input_mode;
    }
    if (address == kParamSG7Assign) {
        return kEncoding_input_assignment;
    }
    if (address == kParamSG7Mode) {
        return kEncoding_sg_input_mode;
    }
    if (address == kParamSG8Assign) {
        return kEncoding_input_assignment;
    }
    if (address == kParamSG8Mode) {
        return kEncoding_sg_input_mode;
    }
    if (address == kParamSG9Assign) {
        return kEncoding_input_assignment;
    }
    if (address == kParamSG9Mode) {
        return kEncoding_sg_input_mode;
    }
    if (address == kParamSG10Assign) {
        return kEncoding_input_assignment;
    }
    if (address == kParamSG11Assign) {
        return kEncoding_input_assignment;
    }
    if (address == kParamSG12Assign) {
        return kEncoding_input_assignment;
    }
    if (address == kParamSG13Assign) {
        return kEncoding_input_assignment;
    }
    if (address == kParamTurnBlinkPeriod) {
        return kEncoding_msec;
    }
    if (address == kParamPassingBlinkCount) {
        return kEncoding_up_to_10;
    }
    if (address == kParamPathLightTime) {
        return kEncoding_sec;
    }
    if (address == kParamInteriorLightTime) {
        return kEncoding_sec;
    }
    if (address == kParamWelcomeLightTime) {
        return kEncoding_sec;
    }
    if (address == kParamBrakeBlinkPeriod) {
        return kEncoding_msec;
    }
    if (address == kParamBrakeBlinkCount) {
        return kEncoding_up_to_10;
    }
    if (address == kParamWiperInterval) {
        return kEncoding_msec;
    }
    return kEncoding_none;
}

static const PROGMEM char _param_name_SP1Assign[] = "SP1Assign";
static const PROGMEM char _param_name_SP1Mode[] = "SP1Mode";
static const PROGMEM char _param_name_SP2Assign[] = "SP2Assign";
static const PROGMEM char _param_name_SP2Mode[] = "SP2Mode";
static const PROGMEM char _param_name_SP3Assign[] = "SP3Assign";
static const PROGMEM char _param_name_SP3Mode[] = "SP3Mode";
static const PROGMEM char _param_name_SP4Assign[] = "SP4Assign";
static const PROGMEM char _param_name_SP4Mode[] = "SP4Mode";
static const PROGMEM char _param_name_SP5Assign[] = "SP5Assign";
static const PROGMEM char _param_name_SP5Mode[] = "SP5Mode";
static const PROGMEM char _param_name_SP6Assign[] = "SP6Assign";
static const PROGMEM char _param_name_SP6Mode[] = "SP6Mode";
static const PROGMEM char _param_name_SP7Assign[] = "SP7Assign";
static const PROGMEM char _param_name_SP7Mode[] = "SP7Mode";
static const PROGMEM char _param_name_SG0Assign[] = "SG0Assign";
static const PROGMEM char _param_name_SG0Mode[] = "SG0Mode";
static const PROGMEM char _param_name_SG1Assign[] = "SG1Assign";
static const PROGMEM char _param_name_SG1Mode[] = "SG1Mode";
static const PROGMEM char _param_name_SG2Assign[] = "SG2Assign";
static const PROGMEM char _param_name_SG2Mode[] = "SG2Mode";
static const PROGMEM char _param_name_SG3Assign[] = "SG3Assign";
static const PROGMEM char _param_name_SG3Mode[] = "SG3Mode";
static const PROGMEM char _param_name_SG4Assign[] = "SG4Assign";
static const PROGMEM char _param_name_SG4Mode[] = "SG4Mode";
static const PROGMEM char _param_name_SG5Assign[] = "SG5Assign";
static const PROGMEM char _param_name_SG5Mode[] = "SG5Mode";
static const PROGMEM char _param_name_SG6Assign[] = "SG6Assign";
static const PROGMEM char _param_name_SG6Mode[] = "SG6Mode";
static const PROGMEM char _param_name_SG7Assign[] = "SG7Assign";
static const PROGMEM char _param_name_SG7Mode[] = "SG7Mode";
static const PROGMEM char _param_name_SG8Assign[] = "SG8Assign";
static const PROGMEM char _param_name_SG8Mode[] = "SG8Mode";
static const PROGMEM char _param_name_SG9Assign[] = "SG9Assign";
static const PROGMEM char _param_name_SG9Mode[] = "SG9Mode";
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
param_name(Parameter::Address address)
{
    if (address == kParamSP1Assign) {
        return &_param_name_SP1Assign[0];
    }
    if (address == kParamSP1Mode) {
        return &_param_name_SP1Mode[0];
    }
    if (address == kParamSP2Assign) {
        return &_param_name_SP2Assign[0];
    }
    if (address == kParamSP2Mode) {
        return &_param_name_SP2Mode[0];
    }
    if (address == kParamSP3Assign) {
        return &_param_name_SP3Assign[0];
    }
    if (address == kParamSP3Mode) {
        return &_param_name_SP3Mode[0];
    }
    if (address == kParamSP4Assign) {
        return &_param_name_SP4Assign[0];
    }
    if (address == kParamSP4Mode) {
        return &_param_name_SP4Mode[0];
    }
    if (address == kParamSP5Assign) {
        return &_param_name_SP5Assign[0];
    }
    if (address == kParamSP5Mode) {
        return &_param_name_SP5Mode[0];
    }
    if (address == kParamSP6Assign) {
        return &_param_name_SP6Assign[0];
    }
    if (address == kParamSP6Mode) {
        return &_param_name_SP6Mode[0];
    }
    if (address == kParamSP7Assign) {
        return &_param_name_SP7Assign[0];
    }
    if (address == kParamSP7Mode) {
        return &_param_name_SP7Mode[0];
    }
    if (address == kParamSG0Assign) {
        return &_param_name_SG0Assign[0];
    }
    if (address == kParamSG0Mode) {
        return &_param_name_SG0Mode[0];
    }
    if (address == kParamSG1Assign) {
        return &_param_name_SG1Assign[0];
    }
    if (address == kParamSG1Mode) {
        return &_param_name_SG1Mode[0];
    }
    if (address == kParamSG2Assign) {
        return &_param_name_SG2Assign[0];
    }
    if (address == kParamSG2Mode) {
        return &_param_name_SG2Mode[0];
    }
    if (address == kParamSG3Assign) {
        return &_param_name_SG3Assign[0];
    }
    if (address == kParamSG3Mode) {
        return &_param_name_SG3Mode[0];
    }
    if (address == kParamSG4Assign) {
        return &_param_name_SG4Assign[0];
    }
    if (address == kParamSG4Mode) {
        return &_param_name_SG4Mode[0];
    }
    if (address == kParamSG5Assign) {
        return &_param_name_SG5Assign[0];
    }
    if (address == kParamSG5Mode) {
        return &_param_name_SG5Mode[0];
    }
    if (address == kParamSG6Assign) {
        return &_param_name_SG6Assign[0];
    }
    if (address == kParamSG6Mode) {
        return &_param_name_SG6Mode[0];
    }
    if (address == kParamSG7Assign) {
        return &_param_name_SG7Assign[0];
    }
    if (address == kParamSG7Mode) {
        return &_param_name_SG7Mode[0];
    }
    if (address == kParamSG8Assign) {
        return &_param_name_SG8Assign[0];
    }
    if (address == kParamSG8Mode) {
        return &_param_name_SG8Mode[0];
    }
    if (address == kParamSG9Assign) {
        return &_param_name_SG9Assign[0];
    }
    if (address == kParamSG9Mode) {
        return &_param_name_SG9Mode[0];
    }
    if (address == kParamSG10Assign) {
        return &_param_name_SG10Assign[0];
    }
    if (address == kParamSG11Assign) {
        return &_param_name_SG11Assign[0];
    }
    if (address == kParamSG12Assign) {
        return &_param_name_SG12Assign[0];
    }
    if (address == kParamSG13Assign) {
        return &_param_name_SG13Assign[0];
    }
    if (address == kParamTurnBlinkPeriod) {
        return &_param_name_TurnBlinkPeriod[0];
    }
    if (address == kParamPassingBlinkCount) {
        return &_param_name_PassingBlinkCount[0];
    }
    if (address == kParamPathLightTime) {
        return &_param_name_PathLightTime[0];
    }
    if (address == kParamInteriorLightTime) {
        return &_param_name_InteriorLightTime[0];
    }
    if (address == kParamWelcomeLightTime) {
        return &_param_name_WelcomeLightTime[0];
    }
    if (address == kParamBrakeBlinkPeriod) {
        return &_param_name_BrakeBlinkPeriod[0];
    }
    if (address == kParamBrakeBlinkCount) {
        return &_param_name_BrakeBlinkCount[0];
    }
    if (address == kParamWiperInterval) {
        return &_param_name_WiperInterval[0];
    }
    return nullptr;
}


} // namespace Master

namespace PowerV1
{

bool
param_exists(Parameter::Address address)
{
    if (address == kParamRelay1Status) {
        return true;
    }
    if (address == kParamRelay2Status) {
        return true;
    }
    if (address == kParamRelay3Status) {
        return true;
    }
    if (address == kParamRelay4Status) {
        return true;
    }
    if (address == kParamRelay1Assign) {
        return true;
    }
    if (address == kParamRelay2Assign) {
        return true;
    }
    if (address == kParamRelay3Assign) {
        return true;
    }
    if (address == kParamRelay4Assign) {
        return true;
    }
    return false;
}

uint16_t
param_default(Parameter::Address address)
{
    if (address == kParamRelay1Status) {
        return 0;
    }
    if (address == kParamRelay2Status) {
        return 0;
    }
    if (address == kParamRelay3Status) {
        return 0;
    }
    if (address == kParamRelay4Status) {
        return 0;
    }
    if (address == kParamRelay1Assign) {
        return 0;
    }
    if (address == kParamRelay2Assign) {
        return 0;
    }
    if (address == kParamRelay3Assign) {
        return 0;
    }
    if (address == kParamRelay4Assign) {
        return 0;
    }
    return 0;
}

uint8_t
param_encoding(Parameter::Address address)
{
    if (address == kParamRelay1Status) {
        return kEncoding_v1_output_status;
    }
    if (address == kParamRelay2Status) {
        return kEncoding_v1_output_status;
    }
    if (address == kParamRelay3Status) {
        return kEncoding_v1_output_status;
    }
    if (address == kParamRelay4Status) {
        return kEncoding_v1_output_status;
    }
    if (address == kParamRelay1Assign) {
        return kEncoding_v1_output_assignment;
    }
    if (address == kParamRelay2Assign) {
        return kEncoding_v1_output_assignment;
    }
    if (address == kParamRelay3Assign) {
        return kEncoding_v1_output_assignment;
    }
    if (address == kParamRelay4Assign) {
        return kEncoding_v1_output_assignment;
    }
    return kEncoding_none;
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
param_name(Parameter::Address address)
{
    if (address == kParamRelay1Status) {
        return &_param_name_Relay1Status[0];
    }
    if (address == kParamRelay2Status) {
        return &_param_name_Relay2Status[0];
    }
    if (address == kParamRelay3Status) {
        return &_param_name_Relay3Status[0];
    }
    if (address == kParamRelay4Status) {
        return &_param_name_Relay4Status[0];
    }
    if (address == kParamRelay1Assign) {
        return &_param_name_Relay1Assign[0];
    }
    if (address == kParamRelay2Assign) {
        return &_param_name_Relay2Assign[0];
    }
    if (address == kParamRelay3Assign) {
        return &_param_name_Relay3Assign[0];
    }
    if (address == kParamRelay4Assign) {
        return &_param_name_Relay4Assign[0];
    }
    return nullptr;
}


} // namespace PowerV1

namespace PowerV3
{

bool
param_exists(Parameter::Address address)
{
    if (address == kParamCH1Status) {
        return true;
    }
    if (address == kParamCH2Status) {
        return true;
    }
    if (address == kParamCH3Status) {
        return true;
    }
    if (address == kParamCH4Status) {
        return true;
    }
    if (address == kParamCH5Status) {
        return true;
    }
    if (address == kParamCH1Type) {
        return true;
    }
    if (address == kParamCH2Type) {
        return true;
    }
    if (address == kParamCH3Type) {
        return true;
    }
    if (address == kParamCH4Type) {
        return true;
    }
    if (address == kParamCH5Type) {
        return true;
    }
    if (address == kParamCH1Assign1) {
        return true;
    }
    if (address == kParamCH1Assign2) {
        return true;
    }
    if (address == kParamCH1Assign3) {
        return true;
    }
    if (address == kParamCH1Assign4) {
        return true;
    }
    if (address == kParamCH1PWM1) {
        return true;
    }
    if (address == kParamCH1PWM2) {
        return true;
    }
    if (address == kParamCH1PWM3) {
        return true;
    }
    if (address == kParamCH1PWM4) {
        return true;
    }
    if (address == kParamCH2Assign1) {
        return true;
    }
    if (address == kParamCH2Assign2) {
        return true;
    }
    if (address == kParamCH2Assign3) {
        return true;
    }
    if (address == kParamCH2Assign4) {
        return true;
    }
    if (address == kParamCH2PWM1) {
        return true;
    }
    if (address == kParamCH2PWM2) {
        return true;
    }
    if (address == kParamCH2PWM3) {
        return true;
    }
    if (address == kParamCH2PWM4) {
        return true;
    }
    if (address == kParamCH3Assign1) {
        return true;
    }
    if (address == kParamCH3Assign2) {
        return true;
    }
    if (address == kParamCH3Assign3) {
        return true;
    }
    if (address == kParamCH3Assign4) {
        return true;
    }
    if (address == kParamCH3PWM1) {
        return true;
    }
    if (address == kParamCH3PWM2) {
        return true;
    }
    if (address == kParamCH3PWM3) {
        return true;
    }
    if (address == kParamCH3PWM4) {
        return true;
    }
    if (address == kParamCH4Assign1) {
        return true;
    }
    if (address == kParamCH4Assign2) {
        return true;
    }
    if (address == kParamCH4Assign3) {
        return true;
    }
    if (address == kParamCH4Assign4) {
        return true;
    }
    if (address == kParamCH4PWM1) {
        return true;
    }
    if (address == kParamCH4PWM2) {
        return true;
    }
    if (address == kParamCH4PWM3) {
        return true;
    }
    if (address == kParamCH4PWM4) {
        return true;
    }
    if (address == kParamCH5Assign1) {
        return true;
    }
    if (address == kParamCH5Assign2) {
        return true;
    }
    if (address == kParamCH5Assign3) {
        return true;
    }
    if (address == kParamCH5Assign4) {
        return true;
    }
    if (address == kParamCH5PWM1) {
        return true;
    }
    if (address == kParamCH5PWM2) {
        return true;
    }
    if (address == kParamCH5PWM3) {
        return true;
    }
    if (address == kParamCH5PWM4) {
        return true;
    }
    return false;
}

uint16_t
param_default(Parameter::Address address)
{
    if (address == kParamCH1Status) {
        return 0;
    }
    if (address == kParamCH2Status) {
        return 0;
    }
    if (address == kParamCH3Status) {
        return 0;
    }
    if (address == kParamCH4Status) {
        return 0;
    }
    if (address == kParamCH5Status) {
        return 0;
    }
    if (address == kParamCH1Type) {
        return 0;
    }
    if (address == kParamCH2Type) {
        return 0;
    }
    if (address == kParamCH3Type) {
        return 0;
    }
    if (address == kParamCH4Type) {
        return 0;
    }
    if (address == kParamCH5Type) {
        return 0;
    }
    if (address == kParamCH1Assign1) {
        return 255;
    }
    if (address == kParamCH1Assign2) {
        return 255;
    }
    if (address == kParamCH1Assign3) {
        return 255;
    }
    if (address == kParamCH1Assign4) {
        return 255;
    }
    if (address == kParamCH1PWM1) {
        return 0;
    }
    if (address == kParamCH1PWM2) {
        return 0;
    }
    if (address == kParamCH1PWM3) {
        return 0;
    }
    if (address == kParamCH1PWM4) {
        return 0;
    }
    if (address == kParamCH2Assign1) {
        return 255;
    }
    if (address == kParamCH2Assign2) {
        return 255;
    }
    if (address == kParamCH2Assign3) {
        return 255;
    }
    if (address == kParamCH2Assign4) {
        return 255;
    }
    if (address == kParamCH2PWM1) {
        return 0;
    }
    if (address == kParamCH2PWM2) {
        return 0;
    }
    if (address == kParamCH2PWM3) {
        return 0;
    }
    if (address == kParamCH2PWM4) {
        return 0;
    }
    if (address == kParamCH3Assign1) {
        return 255;
    }
    if (address == kParamCH3Assign2) {
        return 255;
    }
    if (address == kParamCH3Assign3) {
        return 255;
    }
    if (address == kParamCH3Assign4) {
        return 255;
    }
    if (address == kParamCH3PWM1) {
        return 0;
    }
    if (address == kParamCH3PWM2) {
        return 0;
    }
    if (address == kParamCH3PWM3) {
        return 0;
    }
    if (address == kParamCH3PWM4) {
        return 0;
    }
    if (address == kParamCH4Assign1) {
        return 255;
    }
    if (address == kParamCH4Assign2) {
        return 255;
    }
    if (address == kParamCH4Assign3) {
        return 255;
    }
    if (address == kParamCH4Assign4) {
        return 255;
    }
    if (address == kParamCH4PWM1) {
        return 0;
    }
    if (address == kParamCH4PWM2) {
        return 0;
    }
    if (address == kParamCH4PWM3) {
        return 0;
    }
    if (address == kParamCH4PWM4) {
        return 0;
    }
    if (address == kParamCH5Assign1) {
        return 255;
    }
    if (address == kParamCH5Assign2) {
        return 255;
    }
    if (address == kParamCH5Assign3) {
        return 255;
    }
    if (address == kParamCH5Assign4) {
        return 255;
    }
    if (address == kParamCH5PWM1) {
        return 0;
    }
    if (address == kParamCH5PWM2) {
        return 0;
    }
    if (address == kParamCH5PWM3) {
        return 0;
    }
    if (address == kParamCH5PWM4) {
        return 0;
    }
    return 0;
}

uint8_t
param_encoding(Parameter::Address address)
{
    if (address == kParamCH1Status) {
        return kEncoding_v3_output_status;
    }
    if (address == kParamCH2Status) {
        return kEncoding_v3_output_status;
    }
    if (address == kParamCH3Status) {
        return kEncoding_v3_output_status;
    }
    if (address == kParamCH4Status) {
        return kEncoding_v3_output_status;
    }
    if (address == kParamCH5Status) {
        return kEncoding_v3_output_status;
    }
    if (address == kParamCH1Type) {
        return kEncoding_v3_output_type;
    }
    if (address == kParamCH2Type) {
        return kEncoding_v3_output_type;
    }
    if (address == kParamCH3Type) {
        return kEncoding_v3_output_type;
    }
    if (address == kParamCH4Type) {
        return kEncoding_v3_output_type;
    }
    if (address == kParamCH5Type) {
        return kEncoding_v3_output_type;
    }
    if (address == kParamCH1Assign1) {
        return kEncoding_v3_output_assignment;
    }
    if (address == kParamCH1Assign2) {
        return kEncoding_v3_output_assignment;
    }
    if (address == kParamCH1Assign3) {
        return kEncoding_v3_output_assignment;
    }
    if (address == kParamCH1Assign4) {
        return kEncoding_v3_output_assignment;
    }
    if (address == kParamCH1PWM1) {
        return kEncoding_pwm_duty_cycle;
    }
    if (address == kParamCH1PWM2) {
        return kEncoding_pwm_duty_cycle;
    }
    if (address == kParamCH1PWM3) {
        return kEncoding_pwm_duty_cycle;
    }
    if (address == kParamCH1PWM4) {
        return kEncoding_pwm_duty_cycle;
    }
    if (address == kParamCH2Assign1) {
        return kEncoding_v3_output_assignment;
    }
    if (address == kParamCH2Assign2) {
        return kEncoding_v3_output_assignment;
    }
    if (address == kParamCH2Assign3) {
        return kEncoding_v3_output_assignment;
    }
    if (address == kParamCH2Assign4) {
        return kEncoding_v3_output_assignment;
    }
    if (address == kParamCH2PWM1) {
        return kEncoding_pwm_duty_cycle;
    }
    if (address == kParamCH2PWM2) {
        return kEncoding_pwm_duty_cycle;
    }
    if (address == kParamCH2PWM3) {
        return kEncoding_pwm_duty_cycle;
    }
    if (address == kParamCH2PWM4) {
        return kEncoding_pwm_duty_cycle;
    }
    if (address == kParamCH3Assign1) {
        return kEncoding_v3_output_assignment;
    }
    if (address == kParamCH3Assign2) {
        return kEncoding_v3_output_assignment;
    }
    if (address == kParamCH3Assign3) {
        return kEncoding_v3_output_assignment;
    }
    if (address == kParamCH3Assign4) {
        return kEncoding_v3_output_assignment;
    }
    if (address == kParamCH3PWM1) {
        return kEncoding_pwm_duty_cycle;
    }
    if (address == kParamCH3PWM2) {
        return kEncoding_pwm_duty_cycle;
    }
    if (address == kParamCH3PWM3) {
        return kEncoding_pwm_duty_cycle;
    }
    if (address == kParamCH3PWM4) {
        return kEncoding_pwm_duty_cycle;
    }
    if (address == kParamCH4Assign1) {
        return kEncoding_v3_output_assignment;
    }
    if (address == kParamCH4Assign2) {
        return kEncoding_v3_output_assignment;
    }
    if (address == kParamCH4Assign3) {
        return kEncoding_v3_output_assignment;
    }
    if (address == kParamCH4Assign4) {
        return kEncoding_v3_output_assignment;
    }
    if (address == kParamCH4PWM1) {
        return kEncoding_pwm_duty_cycle;
    }
    if (address == kParamCH4PWM2) {
        return kEncoding_pwm_duty_cycle;
    }
    if (address == kParamCH4PWM3) {
        return kEncoding_pwm_duty_cycle;
    }
    if (address == kParamCH4PWM4) {
        return kEncoding_pwm_duty_cycle;
    }
    if (address == kParamCH5Assign1) {
        return kEncoding_v3_output_assignment;
    }
    if (address == kParamCH5Assign2) {
        return kEncoding_v3_output_assignment;
    }
    if (address == kParamCH5Assign3) {
        return kEncoding_v3_output_assignment;
    }
    if (address == kParamCH5Assign4) {
        return kEncoding_v3_output_assignment;
    }
    if (address == kParamCH5PWM1) {
        return kEncoding_pwm_duty_cycle;
    }
    if (address == kParamCH5PWM2) {
        return kEncoding_pwm_duty_cycle;
    }
    if (address == kParamCH5PWM3) {
        return kEncoding_pwm_duty_cycle;
    }
    if (address == kParamCH5PWM4) {
        return kEncoding_pwm_duty_cycle;
    }
    return kEncoding_none;
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
param_name(Parameter::Address address)
{
    if (address == kParamCH1Status) {
        return &_param_name_CH1Status[0];
    }
    if (address == kParamCH2Status) {
        return &_param_name_CH2Status[0];
    }
    if (address == kParamCH3Status) {
        return &_param_name_CH3Status[0];
    }
    if (address == kParamCH4Status) {
        return &_param_name_CH4Status[0];
    }
    if (address == kParamCH5Status) {
        return &_param_name_CH5Status[0];
    }
    if (address == kParamCH1Type) {
        return &_param_name_CH1Type[0];
    }
    if (address == kParamCH2Type) {
        return &_param_name_CH2Type[0];
    }
    if (address == kParamCH3Type) {
        return &_param_name_CH3Type[0];
    }
    if (address == kParamCH4Type) {
        return &_param_name_CH4Type[0];
    }
    if (address == kParamCH5Type) {
        return &_param_name_CH5Type[0];
    }
    if (address == kParamCH1Assign1) {
        return &_param_name_CH1Assign1[0];
    }
    if (address == kParamCH1Assign2) {
        return &_param_name_CH1Assign2[0];
    }
    if (address == kParamCH1Assign3) {
        return &_param_name_CH1Assign3[0];
    }
    if (address == kParamCH1Assign4) {
        return &_param_name_CH1Assign4[0];
    }
    if (address == kParamCH1PWM1) {
        return &_param_name_CH1PWM1[0];
    }
    if (address == kParamCH1PWM2) {
        return &_param_name_CH1PWM2[0];
    }
    if (address == kParamCH1PWM3) {
        return &_param_name_CH1PWM3[0];
    }
    if (address == kParamCH1PWM4) {
        return &_param_name_CH1PWM4[0];
    }
    if (address == kParamCH2Assign1) {
        return &_param_name_CH2Assign1[0];
    }
    if (address == kParamCH2Assign2) {
        return &_param_name_CH2Assign2[0];
    }
    if (address == kParamCH2Assign3) {
        return &_param_name_CH2Assign3[0];
    }
    if (address == kParamCH2Assign4) {
        return &_param_name_CH2Assign4[0];
    }
    if (address == kParamCH2PWM1) {
        return &_param_name_CH2PWM1[0];
    }
    if (address == kParamCH2PWM2) {
        return &_param_name_CH2PWM2[0];
    }
    if (address == kParamCH2PWM3) {
        return &_param_name_CH2PWM3[0];
    }
    if (address == kParamCH2PWM4) {
        return &_param_name_CH2PWM4[0];
    }
    if (address == kParamCH3Assign1) {
        return &_param_name_CH3Assign1[0];
    }
    if (address == kParamCH3Assign2) {
        return &_param_name_CH3Assign2[0];
    }
    if (address == kParamCH3Assign3) {
        return &_param_name_CH3Assign3[0];
    }
    if (address == kParamCH3Assign4) {
        return &_param_name_CH3Assign4[0];
    }
    if (address == kParamCH3PWM1) {
        return &_param_name_CH3PWM1[0];
    }
    if (address == kParamCH3PWM2) {
        return &_param_name_CH3PWM2[0];
    }
    if (address == kParamCH3PWM3) {
        return &_param_name_CH3PWM3[0];
    }
    if (address == kParamCH3PWM4) {
        return &_param_name_CH3PWM4[0];
    }
    if (address == kParamCH4Assign1) {
        return &_param_name_CH4Assign1[0];
    }
    if (address == kParamCH4Assign2) {
        return &_param_name_CH4Assign2[0];
    }
    if (address == kParamCH4Assign3) {
        return &_param_name_CH4Assign3[0];
    }
    if (address == kParamCH4Assign4) {
        return &_param_name_CH4Assign4[0];
    }
    if (address == kParamCH4PWM1) {
        return &_param_name_CH4PWM1[0];
    }
    if (address == kParamCH4PWM2) {
        return &_param_name_CH4PWM2[0];
    }
    if (address == kParamCH4PWM3) {
        return &_param_name_CH4PWM3[0];
    }
    if (address == kParamCH4PWM4) {
        return &_param_name_CH4PWM4[0];
    }
    if (address == kParamCH5Assign1) {
        return &_param_name_CH5Assign1[0];
    }
    if (address == kParamCH5Assign2) {
        return &_param_name_CH5Assign2[0];
    }
    if (address == kParamCH5Assign3) {
        return &_param_name_CH5Assign3[0];
    }
    if (address == kParamCH5Assign4) {
        return &_param_name_CH5Assign4[0];
    }
    if (address == kParamCH5PWM1) {
        return &_param_name_CH5PWM1[0];
    }
    if (address == kParamCH5PWM2) {
        return &_param_name_CH5PWM2[0];
    }
    if (address == kParamCH5PWM3) {
        return &_param_name_CH5PWM3[0];
    }
    if (address == kParamCH5PWM4) {
        return &_param_name_CH5PWM4[0];
    }
    return nullptr;
}


} // namespace PowerV3
