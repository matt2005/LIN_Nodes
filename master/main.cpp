
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "board.h"
#include "lin_protocol.h"
#include "mc33972.h"
#include "softserial.h"

#include "display.h"
#include "master.h"
#include "menu.h"

Board           board;

static void master_init(Display &disp) __attribute__((noinline));
static void master_status(Master &master) __attribute__((noinline));

static void
master_init(Display &disp)
{
    debug("master init");
    // Initialise the display and set the backlight low to avoid
    // overheating the node power supply.
    disp.setBacklight(10);
    disp.writeP(PSTR("Master Node OK"));
    Board::msDelay(2000);
    disp.setBacklight(0);
    disp.clear();

}

static void
master_status(Master &master)
{
    static Timer::Timeval last;

    // don't print too ofte
    if (Timer::timeSince(last) > 2000) {
        last = Timer::timeNow();

        debug("%3u free", Board::freemem());
    }
}

void
main(void)
{
    if (Board::getMode() != 0) {
        // board is in 'recovery' mode
        Board::panic(2);
    }

    MC33972     switches;
    Master      master;
    Display     disp;
    Menu        menu(disp, master);

    // after everything has been constructed...
    debug("%3u free", Board::freemem());

    // enable interrupts; timers and LIN events will start.
    sei();

    // do one-time init
    master_init(disp);

    // spin forever running the UI and polling switches
    for (;;) {
        wdt_reset();
        //menu.tick();
        switches.scan();
#ifdef DEBUG
        master_status(master);
#endif
    }

}

