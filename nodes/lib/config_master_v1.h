#pragma once

// v1 master board
//

#define BOARD_FUNCTION_ID   0

#define pinP0          portB[0]
#define pinP1          portB[1]
#define pinP2          portB[2]
#define pinP3          portB[3]
#define pinP4          portB[4]
#define pinP5          portB[5]
#define pinP6          portB[6]
#define pinP7          portA[7]

#define pinMode1       portB[3] // D7 on the (non-stuffed) LCD connector

#define pinMOSI        portA[4]
#define pinMISO        portA[2]
#define pinSCK         portA[5]
#define pinCS          portA[6] // CS for the switch interface

#define pinLINRX       portA[0]
#define pinLINTX       portA[1]
#define pinLINCS       portB[6]

#define pinDebugTX     portB[2] // D6 on the (non-stuffed) LCD connector

#define paramSPAssign(_n)          Parameter(_n)     // 1...7
#define paramSGAssign(_n)          Parameter(8 + _n) // 8...21
#define paramTurnBlinkPeriod       Parameter(22, 0)
#define paramPassingBlinkCount     Parameter(23, 0)
#define paramPathLightPeriod       Parameter(24, 15)
#define paramInteriorLightPeriod   Parameter(25, 10)
#define paramWelcomeLightPeriod    Parameter(26, 10)
#define paramBrakeBlinkPeriod      Parameter(27, 0)
#define paramBrakeBlinkCount       Parameter(28, 0)
#define paramWiperInterval         Parameter(29, 0)
