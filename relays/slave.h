///@file slave.h

#include "lin_slave.h"

class RelaySlave : public Slave
{
public:
    RelaySlave(uint8_t BoardID);

    bool            testRelay(LIN::RelayID id) 
    {
        return (id < LIN::kRelayMax) && (relayFrame[id / 8] & (1 << id & 0x7));
    }

protected:
    virtual void    headerReceived(LIN::FID fid) override;
    virtual void    responseReceived(LIN::FID fid, LIN::Frame &frame) override;

private:
    volatile LIN::Frame relayFrame;
};
