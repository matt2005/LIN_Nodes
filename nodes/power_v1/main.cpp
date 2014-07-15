
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "board.h"
#include "slave.h"

#include "protocol.h"
#include "param_power_v1.h"

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
    debug("1: %u", paramRelay1Assign.get()); // .get() required to avoid internal compiler error
    debug("2: %u", paramRelay2Assign.get()); // .get() required to avoid internal compiler error
    debug("3: %u", paramRelay3Assign.get()); // .get() required to avoid internal compiler error
    debug("4: %u", paramRelay4Assign.get()); // .get() required to avoid internal compiler error
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

    // init/default parameters
    power_v1ParamAll(init);

    // construct the slave
    RelaySlave  slave(id);
    slave.init();

    // enable interrupts
    sei();

    // run the relay logic forever
    for (;;) {
        wdt_reset();

        // adjust outputs to match our commanded value
        if (slave.test_relay((LIN::RelayID)paramRelay1Assign.get())) {
            pinOUT1.set();

        } else {
            pinOUT1.clear();
        }

        if (slave.test_relay((LIN::RelayID)paramRelay2Assign.get())) {
            pinOUT2.set();

        } else {
            pinOUT2.clear();
        }

        if (slave.test_relay((LIN::RelayID)paramRelay3Assign.get())) {
            pinOUT3.set();

        } else {
            pinOUT3.clear();
        }

        if (slave.test_relay((LIN::RelayID)paramRelay4Assign.get())) {
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

