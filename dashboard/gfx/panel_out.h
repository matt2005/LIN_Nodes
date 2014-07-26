/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#pragma once

#include "graphics.h"
#include "lpc111x.h"

class Panel;

/* abstract panel output driver */
class PanelOut
{
public:
    virtual void    line_off() = 0;
    virtual void    line_update(unsigned row, unsigned slot, FrameBuffer *buffer) = 0;
};

class PanelV2PIO : public PanelOut
{
public:
    PanelV2PIO();
    virtual void    line_off() override;
    virtual void    line_update(unsigned row, unsigned slot, FrameBuffer *buffer) override;
private:
    static const uint16_t   _bR1 = GPIO_IO_P0;
    static const uint16_t   _bG1 = GPIO_IO_P1;
    static const uint16_t   _bB1 = GPIO_IO_P2;
    static const uint16_t   _bR2 = GPIO_IO_P6;
    static const uint16_t   _bG2 = GPIO_IO_P7;
    static const uint16_t   _bB2 = GPIO_IO_P8;
    static const uint16_t   _bA = GPIO_IO_P3;   // to be shared with R1
    static const uint16_t   _bB = GPIO_IO_P9;   // to be shared with G1
    static const uint16_t   _bC = GPIO_IO_P11;  // to be shared with B1
    static const uint16_t   _bD = _bR2;         // shared with R2

    static const uint16_t   _bCLK = GPIO_IO_P4;
    static const uint16_t   _bLAT = GPIO_IO_P5;
    static const uint16_t   _bOE = GPIO_IO_P8;
};
