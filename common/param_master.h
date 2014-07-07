// AUTOMATICALLY GENERATED, DO NOT EDIT
#pragma once

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
#define kParamPathLightPeriod 24
#define kParamInteriorLightPeriod 25
#define kParamWelcomeLightPeriod 26
#define kParamBrakeBlinkPeriod 27
#define kParamBrakeBlinkCount 28
#define kParamWiperInterval 29

#define paramSPAssign1 Parameter(kParamSPAssign1, 0, (LIN::kSwitchIDUnassigned), (LIN::kSwitchIDUnassigned))
#define paramSPAssign2 Parameter(kParamSPAssign2, 0, (LIN::kSwitchIDUnassigned), (LIN::kSwitchIDUnassigned))
#define paramSPAssign3 Parameter(kParamSPAssign3, 0, (LIN::kSwitchIDUnassigned), (LIN::kSwitchIDUnassigned))
#define paramSPAssign4 Parameter(kParamSPAssign4, 0, (LIN::kSwitchIDUnassigned), (LIN::kSwitchIDUnassigned))
#define paramSPAssign5 Parameter(kParamSPAssign5, 0, (LIN::kSwitchIDUnassigned), (LIN::kSwitchIDUnassigned))
#define paramSPAssign6 Parameter(kParamSPAssign6, 0, (LIN::kSwitchIDUnassigned), (LIN::kSwitchIDUnassigned))
#define paramSPAssign7 Parameter(kParamSPAssign7, 0, (LIN::kSwitchIDUnassigned), (LIN::kSwitchIDUnassigned))
#define paramSGAssign0 Parameter(kParamSGAssign0, 0, (LIN::kSwitchIDUnassigned), (LIN::kSwitchIDUnassigned))
#define paramSGAssign1 Parameter(kParamSGAssign1, 0, (LIN::kSwitchIDUnassigned), (LIN::kSwitchIDUnassigned))
#define paramSGAssign2 Parameter(kParamSGAssign2, 0, (LIN::kSwitchIDUnassigned), (LIN::kSwitchIDUnassigned))
#define paramSGAssign3 Parameter(kParamSGAssign3, 0, (LIN::kSwitchIDUnassigned), (LIN::kSwitchIDUnassigned))
#define paramSGAssign4 Parameter(kParamSGAssign4, 0, (LIN::kSwitchIDUnassigned), (LIN::kSwitchIDUnassigned))
#define paramSGAssign5 Parameter(kParamSGAssign5, 0, (LIN::kSwitchIDUnassigned), (LIN::kSwitchIDUnassigned))
#define paramSGAssign6 Parameter(kParamSGAssign6, 0, (LIN::kSwitchIDUnassigned), (LIN::kSwitchIDUnassigned))
#define paramSGAssign7 Parameter(kParamSGAssign7, 0, (LIN::kSwitchIDUnassigned), (LIN::kSwitchIDUnassigned))
#define paramSGAssign8 Parameter(kParamSGAssign8, 0, (LIN::kSwitchIDUnassigned), (LIN::kSwitchIDUnassigned))
#define paramSGAssign9 Parameter(kParamSGAssign9, 0, (LIN::kSwitchIDUnassigned), (LIN::kSwitchIDUnassigned))
#define paramSGAssign10 Parameter(kParamSGAssign10, 0, (LIN::kSwitchIDUnassigned), (LIN::kSwitchIDUnassigned))
#define paramSGAssign11 Parameter(kParamSGAssign11, 0, (LIN::kSwitchIDUnassigned), (LIN::kSwitchIDUnassigned))
#define paramSGAssign12 Parameter(kParamSGAssign12, 0, (LIN::kSwitchIDUnassigned), (LIN::kSwitchIDUnassigned))
#define paramSGAssign13 Parameter(kParamSGAssign13, 0, (LIN::kSwitchIDUnassigned), (LIN::kSwitchIDUnassigned))
#define paramTurnBlinkPeriod Parameter(kParamTurnBlinkPeriod, 0, 255, 0)
#define paramPassingBlinkCount Parameter(kParamPassingBlinkCount, 0, 10, 0)
#define paramPathLightPeriod Parameter(kParamPathLightPeriod, 0, 60, 0)
#define paramInteriorLightPeriod Parameter(kParamInteriorLightPeriod, 0, 60, 0)
#define paramWelcomeLightPeriod Parameter(kParamWelcomeLightPeriod, 0, 60, 0)
#define paramBrakeBlinkPeriod Parameter(kParamBrakeBlinkPeriod, 0, 100, 0)
#define paramBrakeBlinkCount Parameter(kParamBrakeBlinkCount, 0, 10, 0)
#define paramWiperInterval Parameter(kParamWiperInterval, 0, 50, 0)

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
    paramPathLightPeriod.method(__VA_ARGS__); \
    paramInteriorLightPeriod.method(__VA_ARGS__); \
    paramWelcomeLightPeriod.method(__VA_ARGS__); \
    paramBrakeBlinkPeriod.method(__VA_ARGS__); \
    paramBrakeBlinkCount.method(__VA_ARGS__); \
    paramWiperInterval.method(__VA_ARGS__); \
struct hack
