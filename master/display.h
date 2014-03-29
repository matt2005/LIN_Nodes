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
    bool            init();
    
    void            clear();
    void            move(uint8_t x, uint8_t y) { _x = x; _y = y; }
    
    void            write(const char *s) { write(s, readChar); }
    void            writeP(PGM_P s)      { write(s, readCharP); }
    void            write(uint8_t n)     { write(n, 3); }
    void            write(uint16_t n)    { write(n, 5); }

    void            setBacklight(uint8_t value);
    uint8_t         getButtons();

private:
    typedef uint8_t (*Reader)(const char *p);

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

    uint8_t         _x;
    uint8_t         _y;

    bool            send(const uint8_t *pkt);
    bool            recv(uint8_t *pkt, uint8_t pktlen);
    bool            waitAck(uint8_t opcode);
    void            crc(uint8_t *pkt);

    void            write(const char *s, Reader r, uint8_t count = 16);
    void            write(uint16_t n, uint8_t width);

    static uint8_t  readChar(const char *p);
    static uint8_t  readCharP(const char *p);
};
