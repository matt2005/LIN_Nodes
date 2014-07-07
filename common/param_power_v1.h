// AUTOMATICALLY GENERATED, DO NOT EDIT
#pragma once

#define kParamRelay1Assign 1
#define kParamRelay2Assign 2
#define kParamRelay3Assign 3
#define kParamRelay4Assign 4

#define paramRelay1Assign Parameter(kParamRelay1Assign, 0, (LIN::kRelayIDUnassigned), (LIN::kRelayIDUnassigned))
#define paramRelay2Assign Parameter(kParamRelay2Assign, 0, (LIN::kRelayIDUnassigned), (LIN::kRelayIDUnassigned))
#define paramRelay3Assign Parameter(kParamRelay3Assign, 0, (LIN::kRelayIDUnassigned), (LIN::kRelayIDUnassigned))
#define paramRelay4Assign Parameter(kParamRelay4Assign, 0, (LIN::kRelayIDUnassigned), (LIN::kRelayIDUnassigned))

#define paramAll(method, ...) \
    paramRelay1Assign.method(__VA_ARGS__); \
    paramRelay2Assign.method(__VA_ARGS__); \
    paramRelay3Assign.method(__VA_ARGS__); \
    paramRelay4Assign.method(__VA_ARGS__); \
struct hack
