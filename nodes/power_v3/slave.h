/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

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
    virtual bool    st_read_data(uint8_t page, uint8_t index, uint16_t &value) override;
    virtual bool    st_write_data(uint8_t page, uint8_t index, uint16_t value) override;

private:
    LIN::RelayFrame relayFrame;
};
