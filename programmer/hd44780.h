///@file hd44780
///
/// LCD character display driver
///

#pragma once

#include <stdint.h>

#include "print.h"

class HD44780 : public Display
{
public:    
    static void     init();

    virtual void    clear() override;
    virtual void    move(uint8_t x, uint8_t y) override;

protected:
    virtual void    _write(uint8_t c) override;
    
private:
    static const uint8_t    bitE   = (1<<6);
    static const uint8_t    bitRnW = (1<<5);
    static const uint8_t    bitDnC = (1<<4);

    static void     sendCmd(uint8_t cmd);
    static void     sendData(uint8_t cmd);
    static void     sendNibble(uint8_t cmd);
};
