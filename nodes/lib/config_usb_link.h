/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

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
