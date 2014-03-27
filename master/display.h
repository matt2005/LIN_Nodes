///@file display.h
///
/// Interface to the Olimex 16x2 I2C arduino shield display
///

/// XXX see https://github.com/johnmccombs/arduino-libraries/blob/master/Flash/Flash.h

#pragma once

#include <avr/pgmspace.h>
#include "stdint.h"

class Display
{
public:
    bool        init();
    
    void        clear()                         { sendCommand(kLCDClear); _x = _y = 0; }
    void        setBacklight(uint8_t value)     { sendCommand(kSetBacklight, value); }
    uint8_t     getButtons();
    
    void        write(uint8_t x, uint8_t y, char c);
    void        write(char c);
    void        write(uint8_t x, uint8_t y, const char *s);
    void        write(const char *s);
    void        writeP(uint8_t x, uint8_t y, PGM_P s);
    void        writeP(PGM_P s);

private:
    static const uint8_t    kReadAddress    = 0x61;
    static const uint8_t    kWriteAddress   = 0x60;
    static const uint8_t    kBoardID        = 0x65;
    static const uint8_t    kWidth          = 16;
    static const uint8_t    kHeight         = 2;

    enum Command : uint8_t {
        kSetTristate    = 0x01,
        kSetLatch       = 0x02,
        kSetPort        = 0x03,
        kGetButtons     = 0x05,
        kGetID          = 0x20,
        kGetFirmware    = 0x21,
        kLCDClear       = 0x60,
        kLCDWrite       = 0x61,
        kSetBacklight   = 0x62,
        kUARTEnable     = 0x10
    };

    uint8_t     _x;
    uint8_t     _y;

    bool        sendCommand(Command cmd);
    bool        sendCommand(Command cmd, uint8_t param0);
    bool        sendCommand(Command cmd, uint8_t param0, uint8_t param1);
    bool        sendCommand(Command cmd, uint8_t param0, uint8_t param1, uint8_t param2);
    bool        getResponse(uint8_t &value);
};
