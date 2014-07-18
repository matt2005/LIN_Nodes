///@file slave.h

#include "lin_slave.h"

class RelaySlave : public Slave
{
public:
    RelaySlave(uint8_t BoardID);

    bool            test_relay(RelayID id) const
    {
        return (id < kRelayIDMax) && (relayFrame.test(id));
    }

protected:
    virtual void    st_header_received() override;
    virtual void    st_response_received(LIN::Frame &frame) override;
    virtual uint8_t get_param(uint8_t param) override;
    virtual void    set_param(uint8_t param, uint8_t value) override;

private:
    volatile LIN::RelayFrame relayFrame;
};
