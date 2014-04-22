
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "lin_drv.h"
#include "lin_protocol.h"
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

Slave::Slave(LIN::NodeAddress nad) :
    _nad(nad),
    _currentFID(0)
{
    _slave = this;
}

void
Slave::init()
{
    Board::linCS(false);

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
        headerReceived(_currentFID);
        break;

    case LIN_RXOK:
        lin_get_response(_frameBuf.buf());
        Lin_clear_rxok_it();
        responseReceived(_currentFID, _frameBuf);
        _currentFID = 0;
        break;

    case LIN_TXOK:
        Lin_clear_txok_it();
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

Slave::Param
Slave::getParameter(uint8_t id)
{
    if (id >= maxParam) {
        return 0xffff;
    }
    return eeprom_read_word((Param *)(id * sizeof(Param)));
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
        if (_haveSlaveResponse) {
            sendResponse(_slaveResponse, 8);
            _haveSlaveResponse = false;
        }
        break;
    }
}

void
Slave::responseReceived(LIN::FID fid, LIN::Frame &frame)
{
    switch (fid) {
    case LIN::kFIDMasterRequest:
        // check for broadcast sleep request
        if (frame.nad() == LIN::kNADSleep) {
            sleepRequested();
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
    Board::linCS(false);
}

void
Slave::sleepRequested()
{
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