///@file switches.h

#pragma once

#include "board.h"

class MC33972
{
public:
    enum Input : uint8_t
    {
        kInputSG0   = 0,
        kInputSG1,
        kInputSG2,
        kInputSG3,
        kInputSG4,
        kInputSG5,
        kInputSG6,
        kInputSG7,
        kInputSG8,
        kInputSG9,
        kInputSG10,
        kInputSG11,
        kInputSG12,
        kInputSG13,
        kInputSP0,
        kInputSP1,
        kInputSP2,
        kInputSP3,
        kInputSP4,
        kInputSP5,
        kInputSP6,
        kInputSP7,
        kInputMax
    };

    MC33972();

    void                scan() { cmd(kCMDStatus); }
    bool operator[](uint8_t inp) const {
        return (inp < kInputMax) && (_buf[inp / 8] & (1 << (inp & 0x7)));
    }

private:
    enum Command : uint8_t {
        kCMDStatus          = 0x00,
        kCMDSettings        = 0x01,
        kCMDWakeup1         = 0x02,
        kCMDWakeup2         = 0x03,
        kCMDMetallic1       = 0x04,
        kCMDMetallic2       = 0x05,
        kCMDAnalog          = 0x06,
        kCMDWetting1        = 0x07,
        kCMDWetting2        = 0x08,
        kCMDTristate1       = 0x09,
        kCMDTristate2       = 0x0a,
        kCMDCalibration     = 0x0b,
        kCMDSleep           = 0x0c,
        kCMDReset           = 0x7f
    };

    uint8_t                 _buf[3];

    void                    transfer(uint8_t *buf);
    void                    cmd(Command cmd, uint8_t op1 = 0, uint8_t op2 = 0);

    void                    xferWait() { while (!(SPSR & (1 << SPIF))) {} }

};
