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
    
    void        clear();
    void        setBacklight(uint8_t value);
    uint8_t     getButtons();
    
    void        write(uint8_t x, uint8_t y, char c);
    void        write(char c);
    void        write(uint8_t x, uint8_t y, const char *s);
    void        write(const char *s);
    void        writeP(uint8_t x, uint8_t y, PGM_P s);
    void        writeP(PGM_P s);

private:
    static const uint8_t    kReadAddress    = 0x54;
    static const uint8_t    kWriteAddress   = 0x55;
    static const uint8_t    kWidth          = 16;
    static const uint8_t    kHeight         = 2;

    uint8_t     _x;
    uint8_t     _y;

    bool        send(const uint8_t *pkt);
    bool        recv(uint8_t *pkt, uint8_t pktlen);
    void        crc(uint8_t *pkt);
};
