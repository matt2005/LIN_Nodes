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
#define pinSWCLK       portA[6]
#define pinSWCSNS      portA[7]
#define pinSWSYNCB     portB[1]
#define pinSWRSTB      portB[2]

#define pinDebugTX     portB[1] // CSNS_SYNCB can't be used in DEBUG mode...

// each output can have up to 4 relays and associated PWM values assigned to it
// (8 parameters per output)
#define _paramBase(_output)             ((_output) * 8)

#define paramAssign(_output, _index)    (LIN::RelayID)Parameter(_paramBase(_output) + (_index) * 2)
#define paramPWM(_output, _index)       Parameter(_paramBase(_output) + (_index) * 2 + 1)

