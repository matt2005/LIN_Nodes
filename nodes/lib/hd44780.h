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
    HD44780(Dimension size) :
        Display(size)
    {}

    static void     init();

    virtual void    clear() override;
    virtual void    move(Position p) override;

protected:
    virtual void    write(uint8_t c) override;

private:
    static const uint8_t    bitE   = lcdBitE;
    static const uint8_t    bitRnW = lcdBitRnW;
    static const uint8_t    bitDnC = lcdBitDnC;

    static void     send_cmd(uint8_t cmd);
    static void     send_nibble(uint8_t cmd);
};
