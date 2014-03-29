
#include <avr/interrupt.h>

#include "lin_drv.h"
#include "lin_protocol.h"
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

Slave::Slave() :
    _currentFID(0)
{
    _slave = this;
}

void
Slave::init()
{
    pinLINCS.clear();

    lin_init(LIN_2X, LIN_BAUDRATE);
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
        lin_get_response(&_frameBuf.b[0]);
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

void
Slave::requestResponse(uint8_t length)
{
    uint8_t cmode = (_currentFID > 0x3c) ? LIN_1X : LIN_2X;

    lin_rx_response(cmode, length);
}

void
Slave::sendResponse(LIN::Frame &f, uint8_t length)
{
    uint8_t cmode = (_currentFID > LIN::kFIDMasterRequest) ? LIN_1X : LIN_2X;

    pinLINCS.set();
    lin_tx_response(cmode, &f.b[0], length);
}

void
Slave::headerReceived(LIN::FID fid)
{
}

void
Slave::responseReceived(LIN::FID fid, LIN::Frame &frame)
{
}

void
Slave::responseSent(LIN::FID fid)
{
    pinLINCS.clear();
}

