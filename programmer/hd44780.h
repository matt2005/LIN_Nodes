///@file hd44780
///
/// LCD character display driver
///

/// XXX see https://github.com/johnmccombs/arduino-libraries/blob/master/Flash/Flash.h

#pragma once

#include <stdint.h>

#include "print.h"

class HD44780 : public Print
{
public:    
    static void     init();
    static void     clear();
    static void     move(uint8_t x, uint8_t y);

protected:
    virtual void    _write(uint8_t c) override;
    
private:


    static const uint8_t    bitE   = (1<<6);
    static const uint8_t    bitRnW = (1<<5);
    static const uint8_t    bitDnC = (1<<4);

    static const uint8_t    kWidth          = 24;   // XXX parametric
    static const uint8_t    kHeight         = 2;

    static void     sendCmd(uint8_t cmd);
    static void     sendData(uint8_t cmd);
    static void     sendNibble(uint8_t cmd);
};


