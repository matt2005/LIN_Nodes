
#include <avr/io.h>

#include "mc33972.h"

MC33972::MC33972() :
    _buf{0, 0, 0}
{
    // do pin configuration
    pinMOSI.cfgOutput();
    pinMISO.cfgInputNoPull();
    pinSCK.cfgOutput();
    pinCS.set();
    pinCS.cfgOutput();

    // clear flags
    (void)SPSR;
    (void)SPDR;

    // reset the chip to defaults
    cmd(kCMDReset);

    // all SP inputs switch-to-battery
    cmd(kCMDSettings, 0, 0xff);

    // all input pulls enabled
    cmd(kCMDTristate1, 0, 0);
    cmd(kCMDTristate2, 0, 0);

    // all wakeups disabled
    cmd(kCMDWakeup1, 0, 0);
    cmd(kCMDWakeup2, 0, 0);
}

void
MC33972::transfer(uint8_t *buf)
{

    // Configure SPI for mode 2 master at 2MHz
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << CPHA);

    // select the slave
    pinCS.clear();

    SPDR = buf[0];
    xferWait();
    _buf[2] = SPDR;

    SPDR = buf[1];
    xferWait();
    _buf[1] = SPDR;

    SPDR = buf[2];
    xferWait();
    _buf[0] = SPDR;

    pinCS.set();
}

void
MC33972::cmd(Command cmd, uint8_t op1, uint8_t op2)
{
    uint8_t buf[] = { cmd, op1, op2};

    debug("cmd %2x", cmd);
    transfer(&buf[0]);
}
