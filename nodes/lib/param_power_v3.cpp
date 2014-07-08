// AUTOMATICALLY GENERATED, DO NOT EDIT
#include <parameter.h>
#include <lin_protocol.h>
#include <protocol.h>
#include "param_power_v3.h"

PROGMEM const char power_v3ParamNames[] = 
    " \0"
    "CH1 type\0"
    "CH2 type\0"
    "CH3 type\0"
    "CH4 type\0"
    "CH5 type\0"
    "CH1 Assignment 1\0"
    "CH1 Assignment 2\0"
    "CH1 Assignment 3\0"
    "CH1 Assignment 4\0"
    "CH1 PWM 1\0"
    "CH1 PWM 2\0"
    "CH1 PWM 3\0"
    "CH1 PWM 4\0"
    "CH2 Assignment 1\0"
    "CH2 Assignment 2\0"
    "CH2 Assignment 3\0"
    "CH2 Assignment 4\0"
    "CH2 PWM 1\0"
    "CH2 PWM 2\0"
    "CH2 PWM 3\0"
    "CH2 PWM 4\0"
    "CH3 Assignment 1\0"
    "CH3 Assignment 2\0"
    "CH3 Assignment 3\0"
    "CH3 Assignment 4\0"
    "CH3 PWM 1\0"
    "CH3 PWM 2\0"
    "CH3 PWM 3\0"
    "CH3 PWM 4\0"
    "CH4 Assignment 1\0"
    "CH4 Assignment 2\0"
    "CH4 Assignment 3\0"
    "CH4 Assignment 4\0"
    "CH4 PWM 1\0"
    "CH4 PWM 2\0"
    "CH4 PWM 3\0"
    "CH4 PWM 4\0"
    "CH5 Assignment 1\0"
    "CH5 Assignment 2\0"
    "CH5 Assignment 3\0"
    "CH5 Assignment 4\0"
    "CH5 PWM 1\0"
    "CH5 PWM 2\0"
    "CH5 PWM 3\0"
    "CH5 PWM 4\0"
    "\0";

PROGMEM const char power_v3ParamFormats[] = 
    " \0"
    "%T\0"
    "%T\0"
    "%T\0"
    "%T\0"
    "%T\0"
    "%O\0"
    "%O\0"
    "%O\0"
    "%O\0"
    "%u%%\0"
    "%u%%\0"
    "%u%%\0"
    "%u%%\0"
    "%O\0"
    "%O\0"
    "%O\0"
    "%O\0"
    "%u%%\0"
    "%u%%\0"
    "%u%%\0"
    "%u%%\0"
    "%O\0"
    "%O\0"
    "%O\0"
    "%O\0"
    "%u%%\0"
    "%u%%\0"
    "%u%%\0"
    "%u%%\0"
    "%O\0"
    "%O\0"
    "%O\0"
    "%O\0"
    "%u%%\0"
    "%u%%\0"
    "%u%%\0"
    "%u%%\0"
    "%O\0"
    "%O\0"
    "%O\0"
    "%O\0"
    "%u%%\0"
    "%u%%\0"
    "%u%%\0"
    "%u%%\0"
    "\0";

PROGMEM const uint8_t power_v3ParamInfo[] = {
    0, (LIN::kRelayTypeMax-1), LIN::kRelayType5AGeneric,
    0, (LIN::kRelayTypeMax-1), LIN::kRelayType5AGeneric,
    0, (LIN::kRelayTypeMax-1), LIN::kRelayType5AGeneric,
    0, (LIN::kRelayTypeMax-1), LIN::kRelayType5AGeneric,
    0, (LIN::kRelayTypeMax-1), LIN::kRelayType5AGeneric,
    0, (LIN::kRelayIDUnassigned), (LIN::kRelayIDUnassigned),
    0, (LIN::kRelayIDUnassigned), (LIN::kRelayIDUnassigned),
    0, (LIN::kRelayIDUnassigned), (LIN::kRelayIDUnassigned),
    0, (LIN::kRelayIDUnassigned), (LIN::kRelayIDUnassigned),
    0, 100, 0,
    0, 100, 0,
    0, 100, 0,
    0, 100, 0,
    0, (LIN::kRelayIDUnassigned), (LIN::kRelayIDUnassigned),
    0, (LIN::kRelayIDUnassigned), (LIN::kRelayIDUnassigned),
    0, (LIN::kRelayIDUnassigned), (LIN::kRelayIDUnassigned),
    0, (LIN::kRelayIDUnassigned), (LIN::kRelayIDUnassigned),
    0, 100, 0,
    0, 100, 0,
    0, 100, 0,
    0, 100, 0,
    0, (LIN::kRelayIDUnassigned), (LIN::kRelayIDUnassigned),
    0, (LIN::kRelayIDUnassigned), (LIN::kRelayIDUnassigned),
    0, (LIN::kRelayIDUnassigned), (LIN::kRelayIDUnassigned),
    0, (LIN::kRelayIDUnassigned), (LIN::kRelayIDUnassigned),
    0, 100, 0,
    0, 100, 0,
    0, 100, 0,
    0, 100, 0,
    0, (LIN::kRelayIDUnassigned), (LIN::kRelayIDUnassigned),
    0, (LIN::kRelayIDUnassigned), (LIN::kRelayIDUnassigned),
    0, (LIN::kRelayIDUnassigned), (LIN::kRelayIDUnassigned),
    0, (LIN::kRelayIDUnassigned), (LIN::kRelayIDUnassigned),
    0, 100, 0,
    0, 100, 0,
    0, 100, 0,
    0, 100, 0,
    0, (LIN::kRelayIDUnassigned), (LIN::kRelayIDUnassigned),
    0, (LIN::kRelayIDUnassigned), (LIN::kRelayIDUnassigned),
    0, (LIN::kRelayIDUnassigned), (LIN::kRelayIDUnassigned),
    0, (LIN::kRelayIDUnassigned), (LIN::kRelayIDUnassigned),
    0, 100, 0,
    0, 100, 0,
    0, 100, 0,
    0, 100, 0,
};
