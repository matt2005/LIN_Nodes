///@file switches.h

#pragma once

#include "board.h"
#include "lin_slave.h"

class Switches
{
public:
    Switches(uint8_t paramBase = 0);

    void                scan() { cmd(kCMDStatus); }
    bool                operator[](LIN::SwitchID id);

private:
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
        kInputMax,

        kInputNone  = 255
    };

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
    const uint8_t           _paramBase;

    void                    transfer(uint8_t *buf);
    void                    cmd(Command cmd, uint8_t op1 = 0, uint8_t op2 = 0);

    LIN::SwitchID       getAssignment(uint8_t in) const 
    {
        return (LIN::SwitchID)Slave::getParameter(in + _paramBase); 
    }
};
