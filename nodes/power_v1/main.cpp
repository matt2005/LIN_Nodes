/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "board.h"
#include "slave.h"

#include "lin_defs.h"

using namespace PowerV1;

void
main(void)
{
    Board::init();
    uint8_t     id = Board::get_mode();

    // check for recovery mode before constructing anything else
    if (Board::get_mode() == 0) {
        Board::panic(Board::kPanicCodeRecovery);
    }

#ifdef DEBUG
    OSCCAL += 7;    // XXX test board's osc is slow
    debug("1: %u", Parameter(kParamRelay1Assign).get());
    debug("2: %u", Parameter(kParamRelay2Assign).get());
    debug("3: %u", Parameter(kParamRelay3Assign).get());
    debug("4: %u", Parameter(kParamRelay4Assign).get());
#endif

    // power switch control pins
    pinOUT1.clear();
    pinOUT1.cfg_output();
    pinOUT2.clear();
    pinOUT2.cfg_output();
    pinOUT3.clear();
    pinOUT3.cfg_output();
    pinOUT4.clear();
    pinOUT4.cfg_output();

    // power switch status pins
    pinSTATUS1.cfg_input_no_pull();
    pinSTATUS2.cfg_input_no_pull();
    pinSTATUS3.cfg_input_no_pull();
# ifndef DEBUG                      // debug() output on this pin
    pinSTATUS4.cfg_input_no_pull();
# endif

    // init parameters (set to defaults if not valid)
    for (Parameter::Address addr = 0x0400; addr < 0x04ff; addr++) {
        Parameter p(addr);
        uint8_t encoding = param_encoding(addr);

        if ((encoding != kEncoding_none) && Encoding::invalid(encoding, p.get())) {
            p.set(param_default(addr));
        }
    }

    // construct the slave
    RelaySlave  slave(id);
    slave.init();

    // enable interrupts
    sei();

    // run the relay logic forever
    for (;;) {
        wdt_reset();
        slave.tick();

        // adjust outputs to match our commanded value
        if (slave.test_relay(Parameter(kParamRelay1Assign).get())) {
            pinOUT1.set();

        } else {
            pinOUT1.clear();
        }

        if (slave.test_relay(Parameter(kParamRelay2Assign).get())) {
            pinOUT2.set();

        } else {
            pinOUT2.clear();
        }

        if (slave.test_relay(Parameter(kParamRelay3Assign).get())) {
            pinOUT3.set();

        } else {
            pinOUT3.clear();
        }

        if (slave.test_relay(Parameter(kParamRelay4Assign).get())) {
            pinOUT4.set();

        } else {
            pinOUT4.clear();
        }

        // sniff for output faults
        uint8_t outputFault = 0;

        // XXX fault indications lag output changes...
        if (!pinSTATUS1) {
            outputFault |= 1;
        }

        if (!pinSTATUS2) {
            outputFault |= 2;
        }

        if (!pinSTATUS3) {
            outputFault |= 4;
        }

#ifndef DEBUG   // pin shared with debug() output

        if (!pinSTATUS4) {
            outputFault |= 8;
        }

#endif
        // XXX need somewhere to put this...
    }
}

void
Parameter::set(uint16_t value) const
{
    switch (address()) {
    case Generic::kParamBootloaderMode:
        if (value == 0x4f42) {        // 'BO'
            Board::enter_bootloader();
        }
    }

    if ((address() >> 8) == 0x04) {
        uint8_t index = address() & 0xff;
        eeprom_update_word((uint16_t *)(index * 2), value);
    }
}

uint16_t
Parameter::get() const
{
    switch (address()) {
    case Generic::kParamProtocolVersion:
        return 1;

    case Generic::kParamBootloaderMode:
        return 0;

    case Generic::kParamFirmwareVersion:
        return RELEASE_VERSION;

    case Generic::kParamFirmwarePageSize:
        return SPM_PAGESIZE;
    }

    if ((address() >> 8) == 0x04) {
        uint8_t index = address() & 0xff;
        return eeprom_read_word((const uint16_t *)(index * 2));
    }

    return 0;
}
