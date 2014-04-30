
#include <avr/io.h>

#include "mx33972.h"

MC33972::MC33972(uint8_t paramBase) :
    _buf{0, 0, 0}
{
    return;

    // do pin configuration
    pinMOSI.cfgOutput();
    pinMISO.cfgInputNoPull();
    pinSCK.cfgOutput();
    pinCS.set();
    pinCS.cfgOutput();

    // enable SPI at F_CPU / 4 (2MHz) in mode 2
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << CPHA);

    // clear flags
    (void)SPSR;
    (void)SPDR;

    // reset the chip to defaults
    cmd(kCMDReset);

    //cmd(kCMDSettings, 0, 0xff);   Not required as this is the default state

    // clear wakeup bits
    cmd(kCMDWakeup1, 0, 0);
    cmd(kCMDWakeup2, 0, 0);
}

void
MC33972::transfer(uint8_t *buf)
{
    pinCS.clear();
    SPDR = buf[0];
    while (!(SPSR & (1 << SPIF))) {
        // this should never hang
    }
    _buf[2] = SPDR;
    SPDR = buf[1];
    while (!(SPSR & (1 << SPIF))) {
        // this should never hang
    }
    _buf[1] = SPDR;
    SPDR = buf[2];
    while (!(SPSR & (1 << SPIF))) {
        // this should never hang
    }
    _buf[0] = SPDR;
    pinCS.set();
}

void
MC33972::cmd(Command cmd, uint8_t op1, uint8_t op2)
{
    uint8_t buf[] = { cmd, op1, op2};

    transfer(&buf[0]);
}
