#pragma once

//
// v3 power node
//

#define BOARD_FUNCTION_ID   2

#define pinMode1       portB[0]
#define pinMode2       portB[5]
#define pinMode4       portB[3]
#define pinMode8       portB[4]

#define pinLINRX       portA[0]
#define pinLINTX       portA[1]
#define pinLINCS       portB[6]

#define pinMOSI        portA[4]
#define pinMISO        portA[2]
#define pinSCK         portA[5]
#define pinCS          portA[3]
#define pinSWCLK       portB[6]
#define pinSWCSNS      portA[7]
#define pinSWSYNCB     portB[1]
#define pinSWRSTB      portB[2]

#define pinDebugTX     portB[1] // CSNS_SYNCB can't be used in DEBUG mode...

#define ocrSWCLK       3        // TCCR1D bit matching pinSWCLK above
