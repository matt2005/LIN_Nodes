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

static uint8_t      currentNAD;

usbMsgLen_t
usbFunctionSetup(uchar data[8])
{
    usbRequest_t        *rq = (usbRequest_t *)&data[0];
    static uint8_t      status;

    switch (rq->bRequest) {

    case kUSBRequestStatus:
        status = 0;

        if (slave.is_data_ready()) {
            status |= RQ_STATUS_DATA_READY;
        }

        if (slave.is_data_error()) {
            status |= RQ_STATUS_DATA_ERROR;
        }

        usbMsgPtr = &status;
        return sizeof(status);

    case kUSBRequestGetHistory:
        usbMsgPtr = slave.get_history();

        if ((usbMsgPtr != nullptr) &&
            (*usbMsgPtr & RQ_HISTORY_FRAME_VALID)) {
            return (*usbMsgPtr & RQ_HISTORY_RESPONSE_VALID) ? 9 : 1;
        }

        // if no history, no reply
        break;

    case kUSBRequestSelectNode:
        currentNAD = rq->wValue.bytes[0];
        break;

    case kUSBRequestReadData:
        slave.get_data_by_id(currentNAD, rq->wIndex.bytes[0], rq->wIndex.bytes[1]);
        break;

    case kUSBRequestReadResult:
        if (slave.is_data_ready()) {
            usbMsgPtr = (uint8_t *)slave.get_data();
            return sizeof(uint16_t);
        }

        break;

    case kUSBRequestWriteData:
        slave.set_data_by_id(currentNAD, rq->wIndex.bytes[0], rq->wIndex.bytes[1], rq->wValue.word);
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

        // Look for work
    }
}

