
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "board.h"
#include "lin_protocol.h"

#include "display.h"
#include "slave.h"
#include "master.h"
#include "menu.h"

Master          master;
Event           controlsRequest(0, LIN::kFIDControls);
Event           masterRequest(1, LIN::kFIDMasterRequest);
Event           slaveResponse(1, LIN::kFIDSlaveResponse);

MasterSlave     slave;
Display         disp;
Menu            menu(disp);

// for timer testing
//void blink(void *arg) { pinLINCS.toggle(); }

int
main(void)
{
    // basic board & system init
    Board::init();
    if (Board::getMode() != 0) {
        // stop here as we are in 'recovery' mode
        Board::panic(2);
    }
    slave.init();
    Timer::init();
    sei();

    // display init
    if (!disp.init()) {
        Board::panic(3);
    }
    disp.setBacklight(10);
    disp.clear();

    for (unsigned i = 0; i < 5; i++) {
        wdt_reset();
        _delay_ms(100);
    }
    disp.writeP(PSTR("Master Node OK"));
    for (unsigned i = 0; i < 20; i++) {
        wdt_reset();
        _delay_ms(100);
    }

    // spin running the UI
    for (;;) {
        wdt_reset();
        menu.tick();
    }
}


void
MasterSlave::headerReceived(LIN::FID fid)
{
    switch (fid) {
    case LIN::kFIDControls:
        // XXX send a 
    default:
        // we don't respond to the ususal slave messages
        break;
    }
}


void
MasterSlave::responseReceived(LIN::FID fid, LIN::Frame &frame)
{
    switch (fid) {

    default:
        // we don't respond to the ususal slave messages
        break;
    }
}
