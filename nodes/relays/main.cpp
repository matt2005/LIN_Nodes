
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "board.h"
#include "slave.h"

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
    debug("1: %u", paramRelay1Assign.get());
    debug("2: %u", paramRelay2Assign.get());
    debug("3: %u", paramRelay3Assign.get());
    debug("4: %u", paramRelay4Assign.get());
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

    // construct the slave
    RelaySlave  slave(id);
    slave.init();

    // enable interrupts
    sei();

    // run the relay logic forever
    for (;;) {
        wdt_reset();

        // adjust outputs to match our commanded value
        if (slave.testRelay(paramRelay1Assign.get())) {
            pinOUT1.set();

        } else {
            pinOUT1.clear();
        }

        if (slave.testRelay(paramRelay2Assign.get())) {
            pinOUT2.set();

        } else {
            pinOUT2.clear();
        }

        if (slave.testRelay(paramRelay3Assign.get())) {
            pinOUT3.set();

        } else {
            pinOUT3.clear();
        }

        if (slave.testRelay(paramRelay4Assign.get())) {
            pinOUT4.set();

        } else {
            pinOUT4.clear();
        }

        // sniff for output faults
        uint8_t outputFault = 0;

        // XXX fault indications lag output changes...
        if (!pinSTATUS1.get()) {
            outputFault |= 1;
        }

        if (!pinSTATUS2.get()) {
            outputFault |= 2;
        }

        if (!pinSTATUS3.get()) {
            outputFault |= 4;
        }

#ifndef DEBUG   // pin shared with debug() output

        if (!pinSTATUS4.get()) {
            outputFault |= 8;
        }

#endif
        // XXX need somewhere to put this...
    }
}

