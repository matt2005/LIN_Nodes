/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

///@file softserial.h

#pragma once

#include "pin.h"
#include "print.h"

class Serial : public Print
{
protected:
    virtual void    write(uint8_t c) override;

private:
    static void     tx(uint8_t c);
    static void     tuned_delay(uint16_t delay);
};
