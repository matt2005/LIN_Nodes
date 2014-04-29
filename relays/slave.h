///@file slave.h

#include "lin_slave.h"

class RelaySlave : public Slave
{
public:
    RelaySlave(uint8_t BoardID);

protected:
    virtual void    headerReceived(LIN::FID fid) override;
    virtual void    responseReceived(LIN::FID fid, LIN::Frame &frame) override;

private:
    Timer           _monitorTimer;

    uint8_t         _relayState;
    uint8_t         _outputFault;

    static void     monitor(void *arg);
};
