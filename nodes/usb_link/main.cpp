
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "board.h"
#include "lin_slave.h"

extern "C" {
#include "usbdrv.h"
}

#define CUSTOM_RQ_ECHO          0
/* Request that the device sends back wValue and wIndex. This is used with
 * random data to test the reliability of the communication.
 */
#define CUSTOM_RQ_SET_STATUS    1
/* Set the LED status. Control-OUT.
 * The requested status is passed in the "wValue" field of the control
 * transfer. No OUT data is sent. Bit 0 of the low byte of wValue controls
 * the LED.
 */

#define CUSTOM_RQ_GET_STATUS    2
/* Get the current LED status. Control-IN.
 * This control transfer involves a 1 byte data phase where the device sends
 * the current status to the host. The status is in bit 0 of the byte.
 */

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
    usbRequest_t    *rq = (usbRequest_t *)&data[0];
    static uchar    dataBuffer[4];  /* buffer must stay valid when usbFunctionSetup returns */

    if (rq->bRequest == CUSTOM_RQ_ECHO) { /* echo -- used for reliability tests */
        dataBuffer[0] = rq->wValue.bytes[0];
        dataBuffer[1] = rq->wValue.bytes[1];
        dataBuffer[2] = rq->wIndex.bytes[0];
        dataBuffer[3] = rq->wIndex.bytes[1];
        usbMsgPtr = dataBuffer;         /* tell the driver which data to return */
        return 4;

    } else if (rq->bRequest == CUSTOM_RQ_SET_STATUS) {
        if (rq->wValue.bytes[0] & 1) {  /* set LED */
            pinLINCS.set();

        } else {                         /* clear LED */
            pinLINCS.clear();
        }

    } else if (rq->bRequest == CUSTOM_RQ_GET_STATUS) {
        dataBuffer[0] = pinLINCS.get();
        usbMsgPtr = dataBuffer;         /* tell the driver which data to return */
        return 1;                       /* tell the driver to send 1 byte */
    }

    return 0;   /* default for not implemented requests: return no data back to host */
}


Slave slave(LIN::kNodeAddressSniffer, true);    //< polled-mode slave driver

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

