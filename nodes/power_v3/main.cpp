
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "board.h"
#include "slave.h"
#include "mc17xsf500.h"

void
main(void)
{
    Board::init();
    uint8_t     id = Board::getMode();

    // check for recovery mode before constructing anything else
    if (Board::getMode() == 0) {
        Board::panic(Board::kPanicRecovery);
    }

    // construct the slave
    RelaySlave  slave(id);
    slave.init();

    // initialise the power switch
    MC17XSF500::configure();

    // enable interrupts
    sei();

    // run the output logic forever
    for (;;) {
        wdt_reset();

        // update outputs (XXX magic numbers)
        for (unsigned output = 0; output < 5; output++) {
            uint8_t duty_cycle = 0;

            for (unsigned function = 0; function < 4; function++) {
                if (slave.testRelay(paramAssign(output, function).get())) {
                    if (paramPWM(output, function).get() > duty_cycle) {
                        duty_cycle = paramPWM(output, function).get();
                    }
                }
            }
            MC17XSF500::set(output, duty_cycle);
        }
    }
}

