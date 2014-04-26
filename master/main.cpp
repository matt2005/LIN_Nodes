
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "board.h"
#include "lin_protocol.h"

#include "display.h"
#include "master.h"
#include "menu.h"
#include "switches.h"
#include "softserial.h"

Board           board;

static void master_init(Display &disp) __attribute__((noinline));
static void main_master() __attribute__((noreturn,noinline));
static void main_slave() __attribute__((noreturn,noinline));
static void master_status(Master &master) __attribute__((noinline));


static void
master_init(Display &disp)
{
    // Initialise the display and set the backlight low to avoid
    // overheating the node power supply.
    disp.setBacklight(10);
    disp.writeP(PSTR("Master Node OK"));
    Board::msDelay(2000);
    disp.setBacklight(0);
    disp.clear();

    // fill (most of) the stack with 0xff for sniffing purposes
    {
        extern uint8_t _end;
        volatile uint8_t *p = &_end;

        while (p < (uint8_t *)SP) {
            *p++ = 0xff;
        }
    }
}

static void
master_status(Master &master)
{
    static Timer::Timeval last;

    // don't print too ofte
    if (Timer::timeSince(last) > 2000) {
        last = Timer::timeNow();

        // approximate free memory value
        extern uint8_t _end;
        volatile uint8_t *p = &_end;
        uint16_t free = 0;

        while ((*p == 0xff) && (p < (uint8_t *)SP)) {
            free++;
            p++;
        }

        debug("%u: %3u free", Timer::timeNow(), free);
        debug("  %u headers %u responseRX %u responseTX", master.nHeader, master.nResponseRx, master.nResponseTx);
    }
}

static void
main_master()
{
    Switches    switches;
    Master      master;
    Display     disp;
    Menu        menu(disp, master);

    // enable interrupts; timers and LIN events will start.
    sei();

    // do one-time init
    master_init(disp);

    // spin forever running the UI and polling switches
    for (;;) {
        wdt_reset();
        menu.tick();
        switches.scan();
#ifdef DEBUG
        master_status(master);
#endif
    }
}

static void
main_slave()
{
    Switches    switches;
    SwitchSlave slave;

    // enable interrupts; timers and LIN events will start.
    sei();

    // spin tickling the watchdog and checking switches
    for (;;) {
        wdt_reset();
        switches.scan();
    }
}

void
main(void)
{
    switch (Board::getMode()) {
    case 0:
        main_master();
    case 8:
        main_slave();
    default:
        // board is in 'recovery' mode
        Board::panic(2);
    }
}

