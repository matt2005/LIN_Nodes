
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
    uint8_t     id = Board::getMode();

    // check for recovery mode before constructing anything else
    if (Board::getMode() == 0) {
        Board::panic(Board::kPanicRecovery);
    }

#ifdef DEBUG
    OSCCAL += 7;    // XXX test board's osc is slow
    debug("1: %u", paramRelay1Assign);
    debug("2: %u", paramRelay2Assign);
    debug("3: %u", paramRelay3Assign);
    debug("4: %u", paramRelay4Assign);
#endif

    // power switch control pins
    pinOUT1.clear();
    pinOUT1.cfgOutput();
    pinOUT2.clear();
    pinOUT2.cfgOutput();
    pinOUT3.clear();
    pinOUT3.cfgOutput();
    pinOUT4.clear();
    pinOUT4.cfgOutput();

    // power switch status pins
    pinSTATUS1.cfgInputNoPull();
    pinSTATUS2.cfgInputNoPull();
    pinSTATUS3.cfgInputNoPull();
# ifndef DEBUG                      // debug() output on this pin
    pinSTATUS4.cfgInputNoPull();
# endif

    // init/default parameters
    paramAll(init);

    // construct the slave
    RelaySlave  slave(id);
    slave.init();

    // enable interrupts
    sei();

    // run the relay logic forever
    for (;;) {
        wdt_reset();

        // adjust outputs to match our commanded value
        if (slave.testRelay((LIN::RelayID)paramRelay1Assign.get())) {
            pinOUT1.set();

        } else {
            pinOUT1.clear();
        }

        if (slave.testRelay((LIN::RelayID)paramRelay2Assign.get())) {
            pinOUT2.set();

        } else {
            pinOUT2.clear();
        }

        if (slave.testRelay((LIN::RelayID)paramRelay3Assign.get())) {
            pinOUT3.set();

        } else {
            pinOUT3.clear();
        }

        if (slave.testRelay((LIN::RelayID)paramRelay4Assign.get())) {
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

