
#include <avr/interrupt.h>

#include "lin_drv.h"
#include "lin_slave.h"
#include "board.h"

static Slave *_slave;

// parameter storage
EEMEM uint16_t Slave::parameters[Slave::maxParam];

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
#ifdef DEBUG
    nHeader(0),
    nResponseRx(0),
    nResponseTx(0),
#endif
    _nad(nad),
    _idleTimer(&Slave::idleTimeout),
    _currentFID(LIN::kFIDNone)
{
    _slave = this;

    lin_init(LIN_2X, CONF_LINBRR);
    Lin_set_enable_it();
}

void
Slave::isrTC() 
{
    switch (Lin_get_it()) {
    case LIN_IDOK:
        _currentFID = Lin_get_id();
        Lin_clear_idok_it();
#ifdef DEBUG
        nHeader++;
#endif
        headerReceived(_currentFID);
        break;

    case LIN_RXOK:
    {
        LIN::Frame f;

        lin_get_response(f.buf());
        Lin_clear_rxok_it();
#ifdef DEBUG
        nResponseRx++;
#endif
        responseReceived(_currentFID, f);
        _currentFID = 0;
        break;
    }

    case LIN_TXOK:
        Lin_clear_txok_it();
#ifdef DEBUG
        nResponseTx++;
#endif
        responseSent(_currentFID);
        _currentFID = 0;
        break;
    }
}

void
Slave::isrError()
{
    // XXX we should really clear some state here based on what went wrong,
    //     but it looks like transmission may still be in progress and so 
    //     we run the risk of clearing LINCS and thus powering down the board
    //     on the next 0-bit transmitted.
    //
    // For now, we just ignore the error.

    // clear the interrupt
    Lin_clear_err_it();
}

void
Slave::requestResponse(uint8_t length)
{
    lin_rx_response(LIN_2X, length);
}

void
Slave::sendResponse(LIN::Frame &f, uint8_t length)
{
    // turn on the LIN transmitter
    Board::linCS(true);

    // and send the response
    lin_tx_response(LIN_2X, f.buf(), length);
}

void
Slave::headerReceived(LIN::FID fid)
{
    switch (fid) {
    case LIN::kFIDSlaveResponse:
        if (_slaveResponse.sid() & LIN::kSIDResponseOffset) {
            sendResponse(_slaveResponse, 8);
            _slaveResponse.sid() = 0;
        }
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
            LIN::Frame f(6,
                         frame.sid() | LIN::kSIDResponseOffset,
                         LIN::kSupplierID & 0xff,
                         LIN::kSupplierID >> 8,
                         1);
            prepareSlaveResponse(f);

        // serial number    
        } else if (frame.d1() == 1) {
            LIN::Frame f(5, frame.sid() | LIN::kSIDResponseOffset);
            prepareSlaveResponse(f);
        }

        // nothing to do here
        return;
    }
}

void
Slave::setParameter(uint8_t id, Param value)
{
    if (id < maxParam) {
        eeprom_update_word((Param *)(id * sizeof(Param)), value);
    }
}
