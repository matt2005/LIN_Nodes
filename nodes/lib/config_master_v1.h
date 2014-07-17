#pragma once

// v1 master board
//

static const uint8_t kBoardFunctionID = 0;


#define pinMode1        portA[5]    // SCK/ENCBUT pulled low will force recovery mode

#define pinMOSI         portA[4]
#define pinMISO         portA[2]
#define pinSCK          portA[5]
#define pinCS           portA[6]    // CS for the switch interface

#define pinLINRX        portA[0]
#define pinLINTX        portA[1]
#define pinLINCS        portB[6]

#define pinDebugTX      portB[2]    // D6 on the LCD connector
#define pinDebugStrobe  portB[3]    // D7 on the LCD connector
