
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "lin_drv.h"
#include "lin_slave.h"
#include "board.h"

static Slave *_slave;

ISR(LIN_TC_vect)
{
    _slave->isrTC();
}

ISR(LIN_ERR_vect)
{
    _slave->isrError();
}

void
Slave::idleTimeout(void *_arg)
{
    _slave->sleepRequested(kSleepIdle);
}

Slave::Slave(LIN::NodeAddress nad) :
    _nad(nad),
    _idleTimer(&Slave::idleTimeout),
    _currentFID(LIN::kFIDNone)
{
    _slave = this;

    for (uint8_t i = 0; i < kErrMax; i++) {
        errors[i] = 0;
    }

    // LINCS/LINTX start tristated, configure for LIN use.
    pinLINCS.set();
    pinLINCS.cfgOutput();
    pinLINTX.cfgInputPullUp();
    pinLINRX.cfgInputPullUp();

    // Reset the LIN block
    Lin_full_reset();
    Lin_set_baudrate(CONF_LINBRR);
    Lin_2x_enable();

    // Enable interrupts (note, TXOK currently disabled)
    Lin_set_enable_it();

}

void
Slave::masterTest()
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
Slave::isrTC() 
{
    if (Is_lin_header_ready()) {

        // get the FID and ack the interrupt
        _currentFID = Lin_get_id();
        Lin_clear_idok_it();

        // handle the header
        headerReceived(_currentFID);

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
        responseReceived(_currentFID, f);

        // clear the current FID, since we have seen its frame
        _currentFID = LIN::kFIDNone;

    }

    if (LINSIR & (1 << LTXOK)) {

        responseSent(_currentFID);

        // clear the current FID, since we have sent its frame
        _currentFID = LIN::kFIDNone;

        // and clear the interrupt
        Lin_clear_txok_it();
    }
}

void
Slave::isrError()
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
Slave::requestResponse(uint8_t length)
{
    waitBusy();

    // select 1x mode for FIDs that require it
    if (_currentFID >= LIN::kFIDMasterRequest) {
        Lin_1x_set_type();
    } else {
        Lin_2x_set_type();
        Lin_set_rx_len(length);
    }

    // and configure to receive the response
    Lin_rx_response();
}

void
Slave::sendHeader(LIN::FrameID fid)
{
    if (LINSIR & (1 << LBUSY)) {
        debug("TX header while busy LINSIR=%2x LINERR=%2x", LINSIR, Lin_get_error_status());
        Board::panic(5);
        return;
    }

    // turn on the LIN transmitter - must not do this while we
    // might possibly still be transmitting
    Board::linCS(true);

    // select 1x or 2x mode
    if (_currentFID >= LIN::kFIDMasterRequest) {
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
Slave::sendResponse(LIN::Frame &f, uint8_t length)
{
    if (LINSIR & (1 << LBUSY)) {
        debug("TX response while busy LINSIR=%2x LINERR=%2x", LINSIR, LINERR);
        Board::panic(5);
        return;
    }

    // turn on the LIN transmitter
    Board::linCS(true);

    // select 1x mode for FIDs that require it
    if (_currentFID >= LIN::kFIDMasterRequest) {
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
Slave::headerReceived(LIN::FID fid)
{
    switch (fid) {
    case LIN::kFIDMasterRequest:
        requestResponse(8);
        break;

    case LIN::kFIDSlaveResponse:
        if (_slaveResponse.sid() & LIN::kSIDResponseOffset) {
            sendResponse(_slaveResponse, 8);
            _slaveResponse.sid() = 0;
        }
        break;

    default:
        break;
    }

    // reset the idle timeout
    _idleTimer.setRemaining(kIdleTimeout);
}

void
Slave::responseReceived(LIN::FID fid, LIN::Frame &frame)
{
    switch (fid) {
    case LIN::kFIDMasterRequest:
        // check for broadcast sleep request
        if (frame.nad() == LIN::kNADSleep) {
            sleepRequested(kSleepRequested);
        }
        // check for directly addressed or broadcast master request
        if ((frame.nad() == _nad) ||
            (frame.nad() == LIN::kNADBroadcast)) {
            masterRequest(frame);
        }
        break;

    default:
        break;
    }
}

void
Slave::responseSent(LIN::FID fid)
{
    // It would be nice to do this, but we seem to be called
    // too soon...
    //Board::linCS(false);
}

void
Slave::sleepRequested(SleepType type)
{
    // default behaviour is to behave as requested
    Board::sleep();
}

void
Slave::masterRequest(LIN::Frame &frame)
{
    // ReadByID
    if (frame.sid() == LIN::kSIDReadByID) {
        // product ID
        if (frame.d1() == 0) {
                frame.pci() = 6;
                frame.sid() |= LIN::kSIDResponseOffset;
                frame.d1() = LIN::kSupplierID & 0xff;
                frame.d2() = LIN::kSupplierID >> 8;
                frame.d3() = 1;
                frame.d4() = 0;
                frame.d5() = 0;

        // serial number    
        } else if (frame.d1() == 1) {
            frame.pci() = 5;
            frame.sid() |= LIN::kSIDResponseOffset;
            frame.d1() = 0x01;
            frame.d2() = 0x02;
            frame.d3() = 0x03;
            frame.d4() = 0x04;
            frame.d5() = 0xff;
        } else {
            // nothing to do here
            return;
        }
        prepareSlaveResponse(frame);
    }
}

void
Slave::waitBusy()
{
    Timer::Timeval then = Timer::timeNow();

    while (LINSIR & (1 << LBUSY)) {
        wdt_reset();
        if (Timer::timeSince(then) > 100) {
            debug("LIN busy lockup");
            Board::panic(6);
        }
    }
}
