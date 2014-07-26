/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */


#include <avr/io.h>

#include "mc33972.h"
#include "board.h"

namespace MC33972
{

enum Command : uint8_t {
    kCMDStatus          = 0x00,
    kCMDSettings        = 0x01,
    kCMDWakeup1         = 0x02,
    kCMDWakeup2         = 0x03,
    kCMDMetallic1       = 0x04,
    kCMDMetallic2       = 0x05,
    kCMDAnalog          = 0x06,
    kCMDWetting1        = 0x07,
    kCMDWetting2        = 0x08,
    kCMDTristate1       = 0x09,
    kCMDTristate2       = 0x0a,
    kCMDCalibration     = 0x0b,
    kCMDSleep           = 0x0c,
    kCMDReset           = 0x7f
};

uint8_t                 _buf[3];
bool                    changed;

static void cmd(Command cmd, uint8_t op1 = 0, uint8_t op2 = 0);
static void wait();

void
configure()
{
    pinMOSI.cfg_output();
    pinMISO.cfg_input_no_pull();
    pinSCK.cfg_output();
    pinCS.set();
    pinCS.cfg_output();

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

void scan()
{
    uint8_t tbuf[3] = {_buf[0], _buf[1], _buf[2]};

    cmd(kCMDStatus);
    changed = tbuf != _buf;
}

bool test(uint8_t inp)
{
    return (inp < kInputMax) && (_buf[inp / 8] & (1 << (inp & 0x7)));
}

void
cmd(Command cmd, uint8_t op1, uint8_t op2)
{
    // Configure SPI for mode 2 master at 2MHz
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << CPHA);

    // clear flags ready for use
    (void)SPSR;
    (void)SPDR;

    // select the slave
    pinCS.clear();

    SPDR = cmd;

    wait();
    _buf[2] = SPDR;

    SPDR = op1;
    wait();
    _buf[1] = SPDR;

    SPDR = op2;
    wait();
    _buf[0] = SPDR;

    pinCS.set();
}

static void
wait() 
{
    while (!(SPSR & (1 << SPIF))) {
        if (!(SPCR & (1 << MSTR))) {
            debug("SPI !MSTR");
            Board::panic(Board::kPanicCodeSPI);
        }
        if (SPSR & (1 << WCOL)) {
            debug("SPI WCOL");
            Board::panic(Board::kPanicCodeSPI);
        }
    }
}

} // namespace MC33972
