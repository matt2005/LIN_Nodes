
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "board.h"
#include "slave.h"

void
main(void)
{
    Board       board;
    uint8_t     id = Board::getMode();

    // check for recovery mode before constructing anything else
    if (Board::getMode() == 0) {
        Board::panic(Board::kPanicRecovery);
    }

    // do pin init
    pinOUT1.clear();
    pinOUT1.cfgOutput();
    pinOUT2.clear();
    pinOUT2.cfgOutput();
    pinOUT3.clear();
    pinOUT3.cfgOutput();
    pinOUT4.clear();
    pinOUT4.cfgOutput();

    pinSTATUS1.cfgInputNoPull();
    pinSTATUS2.cfgInputNoPull();
    pinSTATUS3.cfgInputNoPull();
#ifndef DEBUG                       // debug() output on this pin
    pinSTATUS4.cfgInputNoPull();
#endif

    // construct the slave
    RelaySlave  slave(id);

    // enable interrupts
    sei();

    // run the relay logic forever
    for (;;) {
        wdt_reset();

        // adjust outputs to match our commanded value
        if (slave.testRelay((LIN::RelayID)Parameter(1).get())) {
            pinOUT1.set();
        } else {
            pinOUT1.clear();
        }
        if (slave.testRelay((LIN::RelayID)Parameter(2).get())) {
            pinOUT2.set();
        } else {
            pinOUT2.clear();
        }
        if (slave.testRelay((LIN::RelayID)Parameter(3).get())) {
            pinOUT3.set();
        } else {
            pinOUT3.clear();
        }
        if (slave.testRelay((LIN::RelayID)Parameter(4).get())) {
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

