// AUTOMATICALLY GENERATED, DO NOT EDIT
#pragma once
#include <stdint.h>
#include <parameter.h>

extern const uint8_t power_v1ParamInfo[];
extern const char power_v1ParamNames[];
extern const char power_v1ParamFormats[];

#define kParamRelay1Assign 1
#define kParamRelay2Assign 2
#define kParamRelay3Assign 3
#define kParamRelay4Assign 4
#define kParamMax 5

#define power_v1Param(_index) Parameter((_index), &power_v1ParamInfo[(_index) * 3])

#define paramRelay1Assign power_v1Param(kParamRelay1Assign)
#define paramRelay2Assign power_v1Param(kParamRelay2Assign)
#define paramRelay3Assign power_v1Param(kParamRelay3Assign)
#define paramRelay4Assign power_v1Param(kParamRelay4Assign)

#define paramAll(method, ...) \
    paramRelay1Assign.method(__VA_ARGS__); \
    paramRelay2Assign.method(__VA_ARGS__); \
    paramRelay3Assign.method(__VA_ARGS__); \
    paramRelay4Assign.method(__VA_ARGS__); \
struct hack
