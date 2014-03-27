///@file lin_slave.h
///
/// LIN driver wrapper for slaves
///

#pragma once

#include "lin_protocol.h"

class Slave
{
public:
    Slave();

    void            init();

    void            isrTC();
    void            isrError();

protected:

    void            requestResponse(uint8_t length);
    void            sendResponse(LIN::Frame &frame, uint8_t length);

    virtual void    headerReceived(LIN::FID fid);
    virtual void    responseReceived(LIN::FID fid, LIN::Frame &frame);
    virtual void    responseSent(LIN::FID fid);

private:
    LIN::FID    _currentFID;
    LIN::Frame  _frameBuf;
};
