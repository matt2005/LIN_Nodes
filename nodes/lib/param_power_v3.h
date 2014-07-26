// AUTOMATICALLY GENERATED, DO NOT EDIT
#pragma once
#include <stdint.h>
#include <parameter.h>

extern const uint8_t power_v3ParamInfo[];
extern const char power_v3ParamNames[];
extern const char power_v3ParamFormats[];

#define kParamCH1Type 1
#define kParamCH2Type 2
#define kParamCH3Type 3
#define kParamCH4Type 4
#define kParamCH5Type 5
#define kParamCH1Assign1 6
#define kParamCH1Assign2 7
#define kParamCH1Assign3 8
#define kParamCH1Assign4 9
#define kParamCH1PWM1 10
#define kParamCH1PWM2 11
#define kParamCH1PWM3 12
#define kParamCH1PWM4 13
#define kParamCH2Assign1 14
#define kParamCH2Assign2 15
#define kParamCH2Assign3 16
#define kParamCH2Assign4 17
#define kParamCH2PWM1 18
#define kParamCH2PWM2 19
#define kParamCH2PWM3 20
#define kParamCH2PWM4 21
#define kParamCH3Assign1 22
#define kParamCH3Assign2 23
#define kParamCH3Assign3 24
#define kParamCH3Assign4 25
#define kParamCH3PWM1 26
#define kParamCH3PWM2 27
#define kParamCH3PWM3 28
#define kParamCH3PWM4 29
#define kParamCH4Assign1 30
#define kParamCH4Assign2 31
#define kParamCH4Assign3 32
#define kParamCH4Assign4 33
#define kParamCH4PWM1 34
#define kParamCH4PWM2 35
#define kParamCH4PWM3 36
#define kParamCH4PWM4 37
#define kParamCH5Assign1 38
#define kParamCH5Assign2 39
#define kParamCH5Assign3 40
#define kParamCH5Assign4 41
#define kParamCH5PWM1 42
#define kParamCH5PWM2 43
#define kParamCH5PWM3 44
#define kParamCH5PWM4 45
#define kPower_v3ParamMax 45
#define power_v3Param(_index) Parameter((_index), &power_v3ParamInfo[(_index) * 3])

#define paramCH1Type power_v3Param(kParamCH1Type)
#define paramCH2Type power_v3Param(kParamCH2Type)
#define paramCH3Type power_v3Param(kParamCH3Type)
#define paramCH4Type power_v3Param(kParamCH4Type)
#define paramCH5Type power_v3Param(kParamCH5Type)
#define paramCH1Assign1 power_v3Param(kParamCH1Assign1)
#define paramCH1Assign2 power_v3Param(kParamCH1Assign2)
#define paramCH1Assign3 power_v3Param(kParamCH1Assign3)
#define paramCH1Assign4 power_v3Param(kParamCH1Assign4)
#define paramCH1PWM1 power_v3Param(kParamCH1PWM1)
#define paramCH1PWM2 power_v3Param(kParamCH1PWM2)
#define paramCH1PWM3 power_v3Param(kParamCH1PWM3)
#define paramCH1PWM4 power_v3Param(kParamCH1PWM4)
#define paramCH2Assign1 power_v3Param(kParamCH2Assign1)
#define paramCH2Assign2 power_v3Param(kParamCH2Assign2)
#define paramCH2Assign3 power_v3Param(kParamCH2Assign3)
#define paramCH2Assign4 power_v3Param(kParamCH2Assign4)
#define paramCH2PWM1 power_v3Param(kParamCH2PWM1)
#define paramCH2PWM2 power_v3Param(kParamCH2PWM2)
#define paramCH2PWM3 power_v3Param(kParamCH2PWM3)
#define paramCH2PWM4 power_v3Param(kParamCH2PWM4)
#define paramCH3Assign1 power_v3Param(kParamCH3Assign1)
#define paramCH3Assign2 power_v3Param(kParamCH3Assign2)
#define paramCH3Assign3 power_v3Param(kParamCH3Assign3)
#define paramCH3Assign4 power_v3Param(kParamCH3Assign4)
#define paramCH3PWM1 power_v3Param(kParamCH3PWM1)
#define paramCH3PWM2 power_v3Param(kParamCH3PWM2)
#define paramCH3PWM3 power_v3Param(kParamCH3PWM3)
#define paramCH3PWM4 power_v3Param(kParamCH3PWM4)
#define paramCH4Assign1 power_v3Param(kParamCH4Assign1)
#define paramCH4Assign2 power_v3Param(kParamCH4Assign2)
#define paramCH4Assign3 power_v3Param(kParamCH4Assign3)
#define paramCH4Assign4 power_v3Param(kParamCH4Assign4)
#define paramCH4PWM1 power_v3Param(kParamCH4PWM1)
#define paramCH4PWM2 power_v3Param(kParamCH4PWM2)
#define paramCH4PWM3 power_v3Param(kParamCH4PWM3)
#define paramCH4PWM4 power_v3Param(kParamCH4PWM4)
#define paramCH5Assign1 power_v3Param(kParamCH5Assign1)
#define paramCH5Assign2 power_v3Param(kParamCH5Assign2)
#define paramCH5Assign3 power_v3Param(kParamCH5Assign3)
#define paramCH5Assign4 power_v3Param(kParamCH5Assign4)
#define paramCH5PWM1 power_v3Param(kParamCH5PWM1)
#define paramCH5PWM2 power_v3Param(kParamCH5PWM2)
#define paramCH5PWM3 power_v3Param(kParamCH5PWM3)
#define paramCH5PWM4 power_v3Param(kParamCH5PWM4)

#define power_v3ParamAll(method, ...) \
    paramCH1Type.method(__VA_ARGS__); \
    paramCH2Type.method(__VA_ARGS__); \
    paramCH3Type.method(__VA_ARGS__); \
    paramCH4Type.method(__VA_ARGS__); \
    paramCH5Type.method(__VA_ARGS__); \
    paramCH1Assign1.method(__VA_ARGS__); \
    paramCH1Assign2.method(__VA_ARGS__); \
    paramCH1Assign3.method(__VA_ARGS__); \
    paramCH1Assign4.method(__VA_ARGS__); \
    paramCH1PWM1.method(__VA_ARGS__); \
    paramCH1PWM2.method(__VA_ARGS__); \
    paramCH1PWM3.method(__VA_ARGS__); \
    paramCH1PWM4.method(__VA_ARGS__); \
    paramCH2Assign1.method(__VA_ARGS__); \
    paramCH2Assign2.method(__VA_ARGS__); \
    paramCH2Assign3.method(__VA_ARGS__); \
    paramCH2Assign4.method(__VA_ARGS__); \
    paramCH2PWM1.method(__VA_ARGS__); \
    paramCH2PWM2.method(__VA_ARGS__); \
    paramCH2PWM3.method(__VA_ARGS__); \
    paramCH2PWM4.method(__VA_ARGS__); \
    paramCH3Assign1.method(__VA_ARGS__); \
    paramCH3Assign2.method(__VA_ARGS__); \
    paramCH3Assign3.method(__VA_ARGS__); \
    paramCH3Assign4.method(__VA_ARGS__); \
    paramCH3PWM1.method(__VA_ARGS__); \
    paramCH3PWM2.method(__VA_ARGS__); \
    paramCH3PWM3.method(__VA_ARGS__); \
    paramCH3PWM4.method(__VA_ARGS__); \
    paramCH4Assign1.method(__VA_ARGS__); \
    paramCH4Assign2.method(__VA_ARGS__); \
    paramCH4Assign3.method(__VA_ARGS__); \
    paramCH4Assign4.method(__VA_ARGS__); \
    paramCH4PWM1.method(__VA_ARGS__); \
    paramCH4PWM2.method(__VA_ARGS__); \
    paramCH4PWM3.method(__VA_ARGS__); \
    paramCH4PWM4.method(__VA_ARGS__); \
    paramCH5Assign1.method(__VA_ARGS__); \
    paramCH5Assign2.method(__VA_ARGS__); \
    paramCH5Assign3.method(__VA_ARGS__); \
    paramCH5Assign4.method(__VA_ARGS__); \
    paramCH5PWM1.method(__VA_ARGS__); \
    paramCH5PWM2.method(__VA_ARGS__); \
    paramCH5PWM3.method(__VA_ARGS__); \
    paramCH5PWM4.method(__VA_ARGS__); \
struct hack
