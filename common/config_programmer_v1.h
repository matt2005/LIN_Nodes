#pragma once

// v1 programmer module using the generic node board
//

#define portLCD         portB

// rotary encoder pins
#define ENC_INT_REG     PCMSK0
#define ENC_INT_BITS    ((1 << PCINT2) | (1 << PCINT3) | (1 << PCINT4))
#define ENC_INT_BIT     (1 << PCIE0)
#define pinENCA         portA[3]
#define pinENCB         portA[2]
#define pinButton       portA[4]

#define pinLINRX        portA[0]
#define pinLINTX        portA[1]
#define pinLINCS        portA[6]

#define pinDebugTX      portA[7]
