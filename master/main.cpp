
#include <util/delay.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "board.h"
#include "lin_protocol.h"

#include "display.h"
#include "slave.h"
#include "master.h"

Master          master;
Event           masterRequest(1, LIN::kFIDMasterRequest);
Event           slaveResponse(1, LIN::kFIDSlaveResponse);

MasterSlave     slave;
Display         disp;

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

    // spin running the UI
    for (;;) {
        _delay_ms(100);
        wdt_reset();
        disp.move(0,0);
        disp.write(Board::getMode());
        //disp.write(Event::count);
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
