///@file display.h
///
/// Interface to the Olimex 16x2 I2C arduino shield display
///

/// XXX see https://github.com/johnmccombs/arduino-libraries/blob/master/Flash/Flash.h

#pragma once

#include <stdint.h>

#include <avr/pgmspace.h>

#include "print.h"

class Display : public Print
{
public:
    Display();
    
    void            clear();
    void            move(uint8_t x, uint8_t y) { _x = x; _y = y; }

    void            setBacklight(uint8_t value);

    enum Button {
        kButtonNone     = 0x00,
        kButtonUp       = 0x01,
        kButtonEnter    = 0x02,
        kButtonCancel   = 0x04,
        kButtonLeft     = 0x08,
        kButtonRight    = 0x10,
        kButtonDown     = 0x20
    };

    /// Returns button(s) that have been pressed since the 
    /// last call.
    ///
    Button         getButtonPress();

protected:
    virtual void   _write(uint8_t c) override;
    
private:

    static const uint8_t    kReadAddress    = 0x55;
    static const uint8_t    kWriteAddress   = 0x54;
    static const uint8_t    kWidth          = 16;
    static const uint8_t    kHeight         = 2;

    static const uint8_t    kOPNop          = 0x00;
    static const uint8_t    kOPClear        = 0x06;
    static const uint8_t    kOPBacklight    = 0x0e;
    static const uint8_t    kOPButtons      = 0x18;
    static const uint8_t    kOPWrite        = 0x1f;

    static const uint8_t    kACK            = 0x40;

    uint8_t         _x:4;
    uint8_t         _y:4;

    bool            send(const uint8_t *pkt);
    bool            recv(uint8_t *pkt, uint8_t pktlen);
    bool            waitAck(uint8_t opcode);
    void            crc(uint8_t *pkt);
};
