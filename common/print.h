///@file print.h

#pragma once

class Print
{
public:
   
    void            write(const char *s);
    void            writeP(PGM_P s);
    void            write(uint8_t n);
    void            write(uint16_t n);

protected:
    typedef uint8_t (*Reader)(const char *p);

    virtual void    _write(const char *s, Reader r);
    void            _write(uint16_t n, uint8_t width);

    static uint8_t  readChar(const char *p);
    static uint8_t  readCharP(const char *p);
};
