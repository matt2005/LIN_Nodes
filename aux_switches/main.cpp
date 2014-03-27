
#include <util/delay.h>
#include <avr/wdt.h>

#include "board.h"
#include "slave.h"
#include "lin_protocol.h"

SwitchSlave    slave;

int
main(void)
{
    // basic board & system init
    Board::init();

    // initialise the slave and start it listening
    slave.init();

    // XXX for now, idle here
    for (;;) {
        _delay_ms(200);
        wdt_reset();
    }
}

void
SwitchSlave::headerReceived(LIN::FID fid)
{
    switch (fid) {

    case LIN::kFIDAuxSwitches:
        reportSwitches();
        break;

    default:
        Slave::headerReceived(fid);
        break;
    }
}

void
SwitchSlave::reportSwitches()
{
    LIN::Frame f;

    f.l[0] = 0;
    f.l[1] = 0;

    for (uint8_t i = 0; i < kNumChannels; i++) {

        // ignore unassigned channels
        if (_config[i] & kConfigUnassigned) {
            continue;
        }

        // set the valid bit for this channel
        f.l[1] |= 1UL << i;

        // if the switch is on, set the value bit
        if (_switches & (1 << i)) {
            f.l[0] |= 1UL << i;
        }
    }

    sendResponse(f, 8);
}
