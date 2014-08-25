/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#pragma once

// v1 programmer module using the generic node board
//

#define portLCD         portB
#define lcdBitE         (1<<6)
#define lcdBitRnW       (1<<5)
#define lcdBitDnC       (1<<4)

// rotary encoder pins
#define ENC_INT_REG     PCMSK0
#define ENC_INT_BITS    ((1 << PCINT2) | (1 << PCINT3) | (1 << PCINT4))
#define ENC_INT_BIT     (1 << PCIE0)
#define pinENCA         portA[3]
#define pinENCB         portA[2]
#define pinButton       portA[4]

#define pinMode1        portA[4]    // if the encoder is pressed at power-on, go to recovery mode

#define pinLINRX        portA[0]
#define pinLINTX        portA[1]
#define pinLINCS        portA[6]

#define pinDebugTX      portA[7]
