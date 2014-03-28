///@file slave.h

#pragma once

#include "lin_slave.h"

class MasterSlave : public Slave
{
public:

protected:
    virtual void headerReceived(LIN::FID fid) override;
    virtual void responseReceived(LIN::FID fid, LIN::Frame &frame) override;

private:
};

