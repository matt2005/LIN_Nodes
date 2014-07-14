
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "board.h"
#include "timer.h"

#include "master.h"
#include "switches.h"
#include "relays.h"

#include "protocol.h"
#include "parameter.h"
#include "param_Master.h"

bool __cxa_guard_acquire() { return true; }
void __cxa_guard_release() {}

Master          gMaster;

void
main(void)
{
    Board::init();

    // check for recovery mode before doing anything else
    if (Board::getMode() != 0) {
        Board::panic(Board::kPanicRecovery);
    }

    // init / default parameters
    masterParamAll(init);

    // initialisation
    gMaster.init();         // on v1 boards, must do this before SPI due to !SS being LINCS
    Switches::init();

    // enable interrupts; timers and LIN events will start.
    sei();

    // run the master logic forever
    for (;;) {
        wdt_reset();
        Switches::scan();

        Relays::tick();
    }
}

