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

#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "lin_dev.h"
#include "board.h"

static LINDev *_dev;

ISR(LIN_TC_vect)
{
    _dev->isr_TC();
}

ISR(LIN_ERR_vect)
{
    _dev->isr_error();
}

LINDev::LINDev(bool polled) :
    _polled(polled),
    _responseCopyBack(false)
{
    _dev = this;
}

void
LINDev::init()
{
    // LINCS/LINTX start tristated, configure for LIN use.
    // XXX should be in the ctor...
    pinLINCS.set();
    pinLINCS.cfg_output();
    pinLINTX.cfg_input_pullup();
    pinLINRX.cfg_input_pullup();

    reinit();
}

void
LINDev::reinit()
{
    // Reset the LIN block
    Lin_full_reset();
    Lin_set_baudrate(CONF_LINBRR);
    Lin_2x_enable();

    // Enable interrupts if we are not running polled
    if (!_polled) {
        Lin_set_enable_it();
    }
}

void
LINDev::tick()
{
    if (_polled) {
        isr_error();
        isr_TC();
    }
}

void
LINDev::isr_TC()
{
    if (Is_lin_header_ready()) {

        // ack the interrupt
        Lin_clear_idok_it();

        // turn off the LIN transmitter
        // (we might have turned it on to send the header)
        Board::lin_CS(false);

        // handle the header
        st_header_received();

    }

    if (Is_lin_rx_response_ready()) {
        // reset the FIFO readout pointer
        Lin_clear_index();

        // copy FIFO data into a response
        Response resp;

        for (uint8_t i = 0; i < 8; i++) {
            resp._bytes[i] = Lin_get_data();
        }

        // ack the interrupt
        Lin_clear_rxok_it();

        // handle the frame
        st_response_received(resp);
    }

    if (LINSIR & (1 << LTXOK)) {

        // turn off the LIN transmitter
        // (we turned it on to send the response)
        Board::lin_CS(false);

        st_response_sent();

        // and clear the interrupt
        Lin_clear_txok_it();

        // if we wanted to also receive this frame...
        if (_responseCopyBack) {
            _responseCopyBack = false;

            // reset the FIFO readout pointer
            Lin_clear_index();

            // copy FIFO data into a response
            Response resp;

            for (uint8_t i = 0; i < 8; i++) {
                resp._bytes[i] = Lin_get_data();
            }

            // handle the frame
            st_response_received(resp);
        }
    }
}

void
LINDev::isr_error()
{
    uint8_t status = Lin_get_error_status();

    if (status != 0) {
        if (status & (1 << LBERR)) {
            errors[kErrorLine]++;
        }

        if (status & (1 << LCERR)) {
            errors[kErrorChecksum]++;
        }

        if (status & (1 << LPERR)) {
            errors[kErrorParity]++;
        }

        if (status & (1 << LFERR)) {
            errors[kErrorFraming]++;
        }

        if (status & (1 << LSERR)) {
            errors[kErrorSynchronisation]++;
        }

        // reset to default mode
        reinit();

        // clear the interrupt
        Lin_clear_err_it();
    }
}

void
LINDev::st_expect_response(uint8_t length)
{
    // may be called more than once, or when also transmitting
    if (LINSIR & (1 << LBUSY)) {

        // are we actually in the process of sending a response?
        if ((LINCR & LIN_CMD_MASK) == LIN_TX_RESPONSE) {
            // arrange for a copy-back after transmission completes
            _responseCopyBack = true;
        }

    } else {
        Lin_set_rx_len(length);
        Lin_rx_response();
    }
}

void
LINDev::mt_send_header(uint8_t fid)
{
    // turn on the LIN transmitter - must not do this while we
    // might possibly still be transmitting
    Board::lin_CS(true);

    // forcibly reset the block to clear pending operation or any other stuck error
    reinit();

    if (LINSIR & (1 << LBUSY)) {
        debug("TX header while busy LINSIR=%2x LINERR=%2x", LINSIR, Lin_get_error_status());
        Board::panic(Board::kPanicCodeLIN);
    }

    // disable frame timeouts
    // XXX revisit?
    LINCR |= (1 << LCONF1);

    // send header
    Lin_2x_set_id(fid);
    Lin_tx_header();
}

void
LINDev::st_send_response(const Response &resp, uint8_t length)
{
    // may be called to send a response after we're already listening for one
    if (LINSIR & (1 << LBUSY)) {
        if ((LINCR & LIN_CMD_MASK) == LIN_RX_RESPONSE) {
            Lin_abort();
            Lin_clear_err_it();
            _responseCopyBack = true;

        } else {
            debug("TX response while busy LINCR=%2x LINSIR=%2x", LINCR, LINSIR);
            Board::panic(Board::kPanicCodeLIN);
        }
    }

    // turn on the LIN transmitter
    Board::lin_CS(true);

    // copy data to FIFO
    Lin_set_tx_len(length);
    Lin_clear_index();

    for (uint8_t i = 0; i < length; i++) {
        Lin_set_data(resp._bytes[i]);
    }

    // send frame
    Lin_tx_response();
}

void
LINDev::st_header_received()
{
}

void
LINDev::st_response_received(Response &resp)
{
}

void
LINDev::st_response_sent()
{
}

