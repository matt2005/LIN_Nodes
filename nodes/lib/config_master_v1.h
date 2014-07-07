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

#define pinMode1       portA[5] // SCK/ENCBUT pulled low will force recovery mode

#define pinMOSI        portA[4]
#define pinMISO        portA[2]
#define pinSCK         portA[5]
#define pinCS          portA[6] // CS for the switch interface

#define pinLINRX       portA[0]
#define pinLINTX       portA[1]
#define pinLINCS       portB[6]

#define pinDebugTX     portB[5] // E on the LCD connector
