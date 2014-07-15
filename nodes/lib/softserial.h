///@file softserial.h

#pragma once

#include "pin.h"
#include "print.h"

class Serial : public Print
{
public:
    Serial();

protected:
    virtual void    write(uint8_t c) override;

private:
    static void     tx(uint8_t c);
    static void     tuned_delay(uint16_t delay);
};
