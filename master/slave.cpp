
#include <avr/interrupt.h>

#include "board.h"
#include "lin_drv.h"

#include "slave.h"

void
SwitchSlave::headerReceived(LIN::FID fid)
{
    switch (fid) {
    case LIN::kFIDControls:
        /// XXX send control state here
        break;

    default:
        Slave::headerReceived(fid);
        break;
    }
}

void
SwitchSlave::responseReceived(LIN::FID fid, LIN::Frame &frame)
{
    switch (fid) {

    default:
        Slave::responseReceived(fid, frame);
        break;
    }
}
