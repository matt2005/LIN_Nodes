#pragma once

// v1 usb link module using the generic node board
//

static const uint8_t kBoardFunctionID = 11;

#define pinLINRX        portA[0]
#define pinLINTX        portA[1]
#define pinLINCS        portA[6]

#define pinDebugTX      portA[7]

#define pinUSBDPlus     portB[6]
#define pinUSBDMinus    portB[3]
