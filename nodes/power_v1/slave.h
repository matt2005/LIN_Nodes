///@file slave.h

#include "lin_slave.h"

class RelaySlave : public Slave
{
public:
    RelaySlave(uint8_t BoardID);

    bool            test_relay(LIN::RelayID id) const
    {
        return (id < LIN::kRelayIDMax) && (relayFrame.test(id));
    }

protected:
    virtual void    header_received(LIN::FrameID fid) override;
    virtual void    response_received(LIN::FrameID fid, LIN::Frame &frame) override;
    virtual uint8_t get_param(uint8_t param);
    virtual void    set_param(uint8_t param, uint8_t value);

private:
    volatile LIN::RelayFrame relayFrame;
};
