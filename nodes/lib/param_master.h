// AUTOMATICALLY GENERATED, DO NOT EDIT
#pragma once
#include <stdint.h>
#include <parameter.h>

extern const uint8_t masterParamInfo[];
extern const char masterParamNames[];
extern const char masterParamFormats[];

#define kParamSPAssign1 1
#define kParamSPAssign2 2
#define kParamSPAssign3 3
#define kParamSPAssign4 4
#define kParamSPAssign5 5
#define kParamSPAssign6 6
#define kParamSPAssign7 7
#define kParamSGAssign0 8
#define kParamSGAssign1 9
#define kParamSGAssign2 10
#define kParamSGAssign3 11
#define kParamSGAssign4 12
#define kParamSGAssign5 13
#define kParamSGAssign6 14
#define kParamSGAssign7 15
#define kParamSGAssign8 16
#define kParamSGAssign9 17
#define kParamSGAssign10 18
#define kParamSGAssign11 19
#define kParamSGAssign12 20
#define kParamSGAssign13 21
#define kParamTurnBlinkPeriod 22
#define kParamPassingBlinkCount 23
#define kParamPathLightTime 24
#define kParamInteriorLightTime 25
#define kParamWelcomeLightTime 26
#define kParamBrakeBlinkPeriod 27
#define kParamBrakeBlinkCount 28
#define kParamWiperInterval 29
#define kParamMax 30

#define masterParam(_index) Parameter((_index), &masterParamInfo[(_index) * 3])

#define paramSPAssign1 masterParam(kParamSPAssign1)
#define paramSPAssign2 masterParam(kParamSPAssign2)
#define paramSPAssign3 masterParam(kParamSPAssign3)
#define paramSPAssign4 masterParam(kParamSPAssign4)
#define paramSPAssign5 masterParam(kParamSPAssign5)
#define paramSPAssign6 masterParam(kParamSPAssign6)
#define paramSPAssign7 masterParam(kParamSPAssign7)
#define paramSGAssign0 masterParam(kParamSGAssign0)
#define paramSGAssign1 masterParam(kParamSGAssign1)
#define paramSGAssign2 masterParam(kParamSGAssign2)
#define paramSGAssign3 masterParam(kParamSGAssign3)
#define paramSGAssign4 masterParam(kParamSGAssign4)
#define paramSGAssign5 masterParam(kParamSGAssign5)
#define paramSGAssign6 masterParam(kParamSGAssign6)
#define paramSGAssign7 masterParam(kParamSGAssign7)
#define paramSGAssign8 masterParam(kParamSGAssign8)
#define paramSGAssign9 masterParam(kParamSGAssign9)
#define paramSGAssign10 masterParam(kParamSGAssign10)
#define paramSGAssign11 masterParam(kParamSGAssign11)
#define paramSGAssign12 masterParam(kParamSGAssign12)
#define paramSGAssign13 masterParam(kParamSGAssign13)
#define paramTurnBlinkPeriod masterParam(kParamTurnBlinkPeriod)
#define paramPassingBlinkCount masterParam(kParamPassingBlinkCount)
#define paramPathLightTime masterParam(kParamPathLightTime)
#define paramInteriorLightTime masterParam(kParamInteriorLightTime)
#define paramWelcomeLightTime masterParam(kParamWelcomeLightTime)
#define paramBrakeBlinkPeriod masterParam(kParamBrakeBlinkPeriod)
#define paramBrakeBlinkCount masterParam(kParamBrakeBlinkCount)
#define paramWiperInterval masterParam(kParamWiperInterval)

#define paramAll(method, ...) \
    paramSPAssign1.method(__VA_ARGS__); \
    paramSPAssign2.method(__VA_ARGS__); \
    paramSPAssign3.method(__VA_ARGS__); \
    paramSPAssign4.method(__VA_ARGS__); \
    paramSPAssign5.method(__VA_ARGS__); \
    paramSPAssign6.method(__VA_ARGS__); \
    paramSPAssign7.method(__VA_ARGS__); \
    paramSGAssign0.method(__VA_ARGS__); \
    paramSGAssign1.method(__VA_ARGS__); \
    paramSGAssign2.method(__VA_ARGS__); \
    paramSGAssign3.method(__VA_ARGS__); \
    paramSGAssign4.method(__VA_ARGS__); \
    paramSGAssign5.method(__VA_ARGS__); \
    paramSGAssign6.method(__VA_ARGS__); \
    paramSGAssign7.method(__VA_ARGS__); \
    paramSGAssign8.method(__VA_ARGS__); \
    paramSGAssign9.method(__VA_ARGS__); \
    paramSGAssign10.method(__VA_ARGS__); \
    paramSGAssign11.method(__VA_ARGS__); \
    paramSGAssign12.method(__VA_ARGS__); \
    paramSGAssign13.method(__VA_ARGS__); \
    paramTurnBlinkPeriod.method(__VA_ARGS__); \
    paramPassingBlinkCount.method(__VA_ARGS__); \
    paramPathLightTime.method(__VA_ARGS__); \
    paramInteriorLightTime.method(__VA_ARGS__); \
    paramWelcomeLightTime.method(__VA_ARGS__); \
    paramBrakeBlinkPeriod.method(__VA_ARGS__); \
    paramBrakeBlinkCount.method(__VA_ARGS__); \
    paramWiperInterval.method(__VA_ARGS__); \
struct hack
