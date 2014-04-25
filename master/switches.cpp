
#include <avr/io.h>

#include "switches.h"

Switches::Switches(uint8_t paramBase) :
    _paramBase(paramBase)
{
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

    // look for the ignition and door inputs, turn off all other
    // wakeup sources
    {
        uint8_t opc[3] = {0, 0, 0};

        // scan assignments
        for (uint8_t i = kInputSP0; i < kInputMax; i++) {

            LIN::SwitchID assignment = getAssignment(i);

            // ignore assignments we don't want to wake us up
            if ((assignment != LIN::kSWIgnition) &&
                (assignment != LIN::kSWDoor)) {
                continue;
            }

            // adjust parameters
            switch(i) {
                case kInputSG0:
                case kInputSG1:
                case kInputSG2:
                case kInputSG3:
                case kInputSG4:
                case kInputSG5:
                case kInputSG6:
                case kInputSG7:
                    opc[0] |= 1 << i;
                    break;

                case kInputSG8:
                case kInputSG9:
                case kInputSG10:
                case kInputSG11:
                case kInputSG12:
                case kInputSG13:
                    opc[1] |= 1 << (i - kInputSG8);
                    break;

                case kInputSP0:
                case kInputSP1:
                case kInputSP2:
                case kInputSP3:
                case kInputSP4:
                case kInputSP5:
                case kInputSP6:
                case kInputSP7:
                    opc[2] |= 1 << (i - kInputSP0);
                    break;

                default:
                    break;
            }
        }
        // clear wakeup bits
        cmd(kCMDWakeup1, 0, opc[2]);
        cmd(kCMDWakeup2, opc[1], opc[0]);
    }
}

bool
Switches::operator[](LIN::SwitchID id)
{
    bool result = false;

    // iterate all inputs
    for (uint8_t i = kInputSG0; i < kInputMax; i++) {

        // compare input assignment with ID
        if (getAssignment(i) == id) {

            // if any assigned input is 'on', the switch is on
            switch(i) {
                case kInputSG0:
                case kInputSG1:
                case kInputSG2:
                case kInputSG3:
                case kInputSG4:
                case kInputSG5:
                case kInputSG6:
                case kInputSG7:
                    result |= (_buf[2] & (1 << i));
                    break;

                case kInputSG8:
                case kInputSG9:
                case kInputSG10:
                case kInputSG11:
                case kInputSG12:
                case kInputSG13:
                case kInputSP0:
                case kInputSP1:
                    result |= (_buf[1] & (1 << (i - kInputSG8)));
                    break;

                case kInputSP2:
                case kInputSP3:
                case kInputSP4:
                case kInputSP5:
                case kInputSP6:
                case kInputSP7:
                    result |= (_buf[0] & (1 << (i - kInputSP2)));
                    break;

                default:
                    break;
            }
        }
    }
    return result;
}

void
Switches::transfer(uint8_t *buf)
{
    pinCS.clear();
    SPDR = buf[0];
    while (!(SPSR & (1 << SPIF))) {
        // this should never hang
    }
    _buf[0] = SPDR;
    SPDR = buf[1];
    while (!(SPSR & (1 << SPIF))) {
        // this should never hang
    }
    _buf[1] = SPDR;
    SPDR = buf[2];
    while (!(SPSR & (1 << SPIF))) {
        // this should never hang
    }
    _buf[2] = SPDR;
    pinCS.set();
}

void
Switches::cmd(Command cmd, uint8_t op1, uint8_t op2)
{
    uint8_t buf[] = { cmd, op1, op2};

    transfer(&buf[0]);
}
