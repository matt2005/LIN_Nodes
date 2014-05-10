
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "lin_dev.h"
#include "board.h"

static LINDev *_dev;

ISR(LIN_TC_vect)
{
    _dev->isrTC();
}

ISR(LIN_ERR_vect)
{
    _dev->isrError();
}

LINDev::LINDev()
{
    _dev = this;
}

void
LINDev::init()
{
    // LINCS/LINTX start tristated, configure for LIN use.
    pinLINCS.set();
    pinLINCS.cfgOutput();
    pinLINTX.cfgInputPullUp();
    pinLINRX.cfgInputPullUp();

    // Reset the LIN block
    Lin_full_reset();
    Lin_set_baudrate(CONF_LINBRR);
    Lin_2x_enable();

    // Enable interrupts
    Lin_set_enable_it();
}

void
LINDev::masterTest()
{
    // XXX testing
    debug("LIN testing");

    // no interrupts
    Lin_clear_enable_it();

    // disable frame timeouts
    LINCR |= (1 << LCONF1);

    // no watchdog
    wdt_disable();

    for (;;) {
        sendHeader(LIN::kFIDTest);

        while (!Is_lin_header_ready()) {
            if (LINSIR & (1 << LERR)) {
                debug("frame TX error %2x", Lin_get_error_status());
                Board::panic(10);
            }
        }

        if (Lin_get_id() != LIN::kFIDTest) {
            debug("FID mismatch, got %2x", Lin_get_id());
            Board::panic(10);
        }

        LIN::Frame f(1,2,3,4,5,6,7,8);
        sendResponse(f, 8);

        while (LINSIR & (1 << LBUSY)) {
            if (LINSIR & (1 << LERR)) {
                debug("frame TX error %2x", Lin_get_error_status());
                Board::panic(10);
            }
        }

        Board::msDelay(100);
    }
}

void
LINDev::isrTC() 
{
    if (Is_lin_header_ready()) {

        // ack the interrupt
        Lin_clear_idok_it();

        // handle the header
        headerReceived(currentFID());

    }

    if (Is_lin_rx_response_ready()) {
        LIN::Frame f;

        // reset the FIFO readout pointer
        Lin_clear_index();

        // copy FIFO data into the frame
        for (uint8_t i = 0; i < 8; i++) {
            f[i] = Lin_get_data();
        }

        // reset to default mode, might have been in 1x mode
        // to handle this frame
        Lin_2x_set_type();

        // ack the interrupt
        Lin_clear_rxok_it();

        // handle the frame
        responseReceived(currentFID(), f);
    }

    if (LINSIR & (1 << LTXOK)) {

        responseSent();

        // and clear the interrupt
        Lin_clear_txok_it();
    }
}

void
LINDev::isrError()
{

    if (Lin_get_error_status() & (1 << LBERR)) {
        errors[kErrLine]++;
    }
    if (Lin_get_error_status() & (1 << LCERR)) {
        errors[kErrChecksum]++;
    }
    if (Lin_get_error_status() & (1 << LPERR)) {
        errors[kErrParity]++;
    }
    if (Lin_get_error_status() & (1 << LFERR)) {
        errors[kErrFraming]++;
    }
    if (Lin_get_error_status() & (1 << LSERR)) {
        errors[kErrSynchronisation]++;
    }

    // reset to default mode
    Lin_2x_enable();

    // clear the interrupt
    Lin_clear_err_it();
}

void
LINDev::requestResponse(uint8_t length)
{
    waitBusy();

    // select 1x mode for FIDs that require it
    if (currentFID() >= LIN::kFIDMasterRequest) {
        Lin_1x_set_type();
    } else {
        Lin_2x_set_type();
        Lin_set_rx_len(length);
    }

    // and configure to receive the response
    Lin_rx_response();
}

void
LINDev::sendHeader(LIN::FrameID fid)
{
    if (LINSIR & (1 << LBUSY)) {
        debug("TX header while busy LINSIR=%2x LINERR=%2x", LINSIR, Lin_get_error_status());
        Board::panic(Board::kPanicLIN);
        return;
    }

    // turn on the LIN transmitter - must not do this while we
    // might possibly still be transmitting
    Board::linCS(true);

    // select 1x or 2x mode
    if (currentFID() >= LIN::kFIDMasterRequest) {
        Lin_1x_set_type();
        Lin_1x_set_id(fid);
        Lin_1x_set_len(8);
    } else {
        Lin_2x_set_type();
        Lin_2x_set_id(fid);
    }

    // send header
    Lin_tx_header();
}

void
LINDev::sendResponse(const LIN::Frame &f, uint8_t length)
{
    if (LINSIR & (1 << LBUSY)) {
        debug("TX response while busy LINSIR=%2x LINERR=%2x", LINSIR, LINERR);
        Board::panic(Board::kPanicLIN);
        return;
    }

    // turn on the LIN transmitter
    Board::linCS(true);

    // select 1x mode for FIDs that require it
    if (currentFID() >= LIN::kFIDMasterRequest) {
        Lin_1x_set_type();
    } else {
        Lin_2x_set_type();
        Lin_set_tx_len(length);
    }

    // copy data to FIFO
    Lin_clear_index();
    for (uint8_t i = 0; i < length; i++) {
        Lin_set_data(f[i]);
    }

    // send frame
    Lin_tx_response();
}

void
LINDev::headerReceived(LIN::FID fid)
{
}

void
LINDev::responseReceived(LIN::FID fid, LIN::Frame &frame)
{
}

void
LINDev::responseSent()
{
}

