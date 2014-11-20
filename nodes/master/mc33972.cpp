/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */


#include <avr/io.h>

#include <board.h>
#include <bitarray.h>
#include <lin_defs.h>

#include "mc33972.h"

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

    // all wakeups disabled
    cmd(kCMDWakeup1, 0, 0);
    cmd(kCMDWakeup2, 0, 0);

    // configure SP inputs
    cmd(kCMDMetallic1, 0, 0);
    cmd(kCMDWetting1, 0, 0xff);
    {
        const uint16_t base = Master::kParamSP1Mode;
        const uint8_t stride = Master::kParamSP2Mode - Master::kParamSP1Mode;
        uint8_t settings = 1;       // SP0 is always ignition -> switch-to-battery
        uint8_t tristate = 0;

        for (uint8_t i = 1; i < 8; i++) {
            Parameter p(base + (stride * i));

            switch (p.get()) {
            case sp_input_type::kSwitchToGround:
                settings |= 0 << i;
                tristate |= 0 << i;
                break;

            case sp_input_type::kSwitchToBattery:
                settings |= 1 << i;
                tristate |= 0 << i;
                break;

            case sp_input_type::kLogic:
                settings |= 1 << i;
                tristate |= 1 << i;
                break;

            case sp_input_type::kInvertedLogic:
                settings |= 0 << i;
                tristate |= 1 << i;
                break;
            }
        }

        cmd(kCMDSettings, 0, settings);
        cmd(kCMDTristate1, 0, tristate);
    }

    // configure SG inputs
    cmd(kCMDMetallic2, 0, 0);
    cmd(kCMDWetting2, 0x3f, 0xff);
    {
        const uint16_t base = Master::kParamSG1Mode;
        const uint8_t stride = Master::kParamSG2Mode - Master::kParamSG1Mode;
        uint16_t tristate = 0;

        for (uint8_t i = 0; i < 14; i++) {
            Parameter p(base + (stride * i));

            switch (p.get()) {
            //case sg_switch_input_type::kSwitchToGround:
            case sg_mixed_input_type::kSwitchToGround:
                tristate |= 0 << i;
                break;

            // case sg_switch_input_type::kLogic:
            case sg_mixed_input_type::kLogic:
            case sg_mixed_input_type::kAnalogNoBias:
            case sg_mixed_input_type::kAnalog2mABias:
            case sg_mixed_input_type::kAnalog16mABias:
                tristate |= 1 << i;
                break;
            }
        }

        cmd(kCMDTristate2, tristate >> 8, tristate & 0xff);
    }
}

void
scan()
{
    uint8_t tbuf[3] = {_buf[0], _buf[1], _buf[2]};

    cmd(kCMDStatus);
    changed = tbuf != _buf;
}

bool
test(uint8_t inp)
{
    Bitarray b(_buf);

    return (inp < kInputMax) && b.test(inp);
}

void
analog_select(uint8_t inp)
{
    const uint16_t base = Master::kParamSG1Mode;
    const uint8_t stride = Master::kParamSG2Mode - Master::kParamSG1Mode;

    if (inp <= kInputMax) {

        uint8_t select = inp + 1;

        Parameter p(base + (stride * inp));

        switch (p.get()) {
        case sg_mixed_input_type::kAnalog2mABias:
            select |= (1 << 5);
            break;

        case sg_mixed_input_type::kAnalog16mABias:
            select |= (1 << 6);
            break;
        }

        cmd(kCMDAnalog, 0, select);
    }
}

void
analog_deselect()
{
    cmd(kCMDAnalog, 0, 0);
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
