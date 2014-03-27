
#include <avr/interrupt.h>

#include "lin_drv.h"
#include "lin_protocol.h"
#include "lin_slave.h"
#include "board.h"

static Slave *_slave;

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

ISR(LIN_TC_vect)
{
    _slave->isrTC();
}

void
Slave::isrTC() 
{
    switch (Lin_get_it()) {
    case LIN_IDOK:
        _currentFID = Lin_get_id();
        Lin_clear_idok_it();
        _slave->headerReceived(_currentFID);
        break;

    case LIN_RXOK:
        lin_get_response(&_frameBuf.b[0]);
        Lin_clear_rxok_it();
        _slave->responseReceived(_currentFID, _frameBuf);
        _currentFID = 0;
        break;

    case LIN_TXOK:
        Lin_clear_txok_it();
        _slave->responseSent(_currentFID);
        _currentFID = 0;
        break;
    }
}

ISR(LIN_ERR_vect)
{
    _slave->isrError();
}

void
Slave::isrError()
{
    // clear any state for in-progress operations
    pinLINCS.clear();
    _slave->responseSent(0);

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

