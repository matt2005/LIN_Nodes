/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "board.h"
#include "slave.h"

#include "requests.h"

extern "C" {
#include "usbdrv.h"
}

ToolSlave slave;    //< polled-mode slave driver

usbMsgLen_t
usbFunctionSetup(uchar data[8])
{
    usbRequest_t        *rq = (usbRequest_t *)&data[0];
    static USBDataIn    reply;

    // reply will always be from the same place
    usbMsgPtr = (unsigned char *)&reply;

    switch (rq->bRequest) {
    case kUSBRequestStatus:
        reply.status.flags = RQ_STATUS_CONNECTED;
        return sizeof(reply.status);

    case kUSBRequestGetHistory:
        if (slave.get_history(reply.history.frame) && 
            (reply.history.frame[0] & RQ_HISTORY_FRAME_VALID)) {
            return (reply.history.frame[0] & RQ_HISTORY_RESPONSE_VALID) ? 9 : 1;
        }
        // if no history, no reply
        break;

    case kUSBRequestReadData:
        // XXX how do we stall until the read completes / times out?
        break;

    case kUSBRequestWriteData:
        // XXX how do we stall until the read completes / times out?
        break;

    case kUSBRequestBeginUpdate:
    case kUSBRequestUpdateData:
    case kUSBRequestFinishUpdate:
    default:
        break;
    }

    return 0;   /* default for not implemented requests: return no data back to host */
}


void
main(void)
{
    Board::init();
    debug("USB Link starting");

    pinLINCS.cfg_output();

    //Board::panic(Board::kPanicCodeRecovery);

    usbInit();
    usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */
    uint8_t i = 0;

    while (--i) {           /* fake USB disconnect for > 250 ms */
        wdt_reset();
        Board::ms_delay(1);
    }

    usbDeviceConnect();

    slave.init();

    sei();

    for (;;) {
        wdt_reset();
        slave.tick();
        usbPoll();
    }
}

