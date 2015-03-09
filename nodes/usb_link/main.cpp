/*
 * Copyright (c) 2012-2015, Mike Smith, <msmith@purgatory.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * o Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "board.h"
#include "slave.h"

#include "requests.h"

extern "C" {
#include "usbdrv.h"
}

ToolSlave slave;    //< polled-mode LIN driver (not really a slave...)

static uint8_t      currentNAD;

usbMsgLen_t
usbFunctionSetup(uchar data[8])
{
    usbRequest_t        *rq = (usbRequest_t *)&data[0];
    static uint8_t      status;

    switch (rq->bRequest) {

    case kUSBRequestStatus:
        status = 0;

        switch (rq->wIndex.bytes[0]) {
        case RQ_STATUS_FLAGS:

            if (slave.is_data_ready()) {
                status |= RQ_STATUS_DATA_READY;
            }

            if (slave.is_data_error()) {
                status |= RQ_STATUS_DATA_ERROR;
            }

            if (slave.is_data_rejected()) {
                status |= RQ_STATUS_DATA_REJECTED;
            }

            if (slave.is_awake()) {
                status |= RQ_STATUS_AWAKE;
            }

            if (slave.is_waiting()) {
                status |= RQ_STATUS_WAITING;
            }

            if (slave.is_master()) {
                status |= RQ_STATUS_MASTER;
            }

            break;

        case RQ_STATUS_FREEMEM:
            status = Board::freemem();
            break;

        case RQ_STATUS_LINERR:
            if (rq->wValue.bytes[0] < LINDev::Error::kErrorMax) {
                status = slave.errors[rq->wValue.bytes[0]];
            }

        default:
            break;
        }

        usbMsgPtr = &status;
        return sizeof(status);

    case kUSBRequestClearStatus:
        switch (rq->wIndex.bytes[0]) {
        case RQ_STATUS_LINERR:
            for (uint8_t i = 0; i < LINDev::Error::kErrorMax; i++) {
                slave.errors[i].reset();
            }

            break;

        default:
            break;
        }

        break;

    case kUSBRequestGetHistory:
        usbMsgPtr = (uint8_t *)slave.get_history();

        if (usbMsgPtr != nullptr) {
            return sizeof(struct RQHistory);
        }

        // if no history, no reply
        break;

    case kUSBRequestSelectNode:
        currentNAD = rq->wValue.bytes[0];
        break;

    case kUSBRequestReadResult:
        if (slave.is_data_ready()) {
            usbMsgPtr = (uint8_t *)slave.get_data();
            return sizeof(uint16_t);
        }

        break;

    case kUSBRequestReadData:
        slave.get_data_by_id(currentNAD, rq->wIndex.word);
        usbDisableAllRequests();
        break;

    case kUSBRequestWriteData:
        slave.set_data_by_id(currentNAD, rq->wIndex.word, rq->wValue.word);
        usbDisableAllRequests();
        break;

    case kUSBRequestSendBulk:
        slave.send_bulk(currentNAD, rq->wValue.bytes[0], rq->wValue.bytes[1], rq->wIndex.bytes[0], rq->wIndex.bytes[1]);
        usbDisableAllRequests();
        break;

    case kUSBRequestEnableMaster:
        if (rq->wValue.bytes[0] != 0) {
            slave.enable_master(true);
            usbDisableAllRequests();

        } else {
            slave.enable_master(false);
        }

        break;

    default:
        break;
    }

    return 0;   /* default for not implemented requests: return no data back to host */
}


void
main(void)
{
    Board::early_init();
    debug("USB Link starting");

    pinLINCS.cfg_output();

    //Board::panic(Board::kPanicCodeRecovery);

    usbInit();
    usbDeviceDisconnect();  /* force re-enumeration, do this while interrupts are disabled! */
    uint8_t i = 0;

    while (--i) {           /* fake USB disconnect for > 250 ms */
        wdt_reset();
        Board::ms_delay(1);
    }

    usbDeviceConnect();

    Timer::init();
    slave.init();

    sei();

    for (;;) {
        wdt_reset();
        slave.tick();

        // busy period has ended, we are ready for the next request
        if (usbAllRequestsAreDisabled() && slave.is_data_idle()) {
            usbEnableAllRequests();
        }

        usbPoll();
    }
}

void
Parameter::set(uint16_t value) const
{
}

uint16_t
Parameter::get() const
{
    return 0;
}
