///@file slave.h

#include "lin_slave.h"

class SwitchSlave : public Slave
{
public:
    SwitchSlave(LIN::NodeAddress nad) :
        Slave(nad)
    {
    }

protected:
    virtual void    headerReceived(LIN::FID fid) override;
    virtual void    responseReceived(LIN::FID fid, LIN::Frame &frame) override;
};
