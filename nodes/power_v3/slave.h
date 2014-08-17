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

    bool            test_relay(uint8_t relay_id)
    {
        // we know the relay frame is just an array of bits with relay 0 as the first bit
        return (_relayFrame._raw >> relay_id) & 1;
    }

protected:
    virtual void    st_header_received() override;
    virtual void    st_response_received(Response &frame) override;
    virtual bool    st_read_data(Parameter::Address address, uint16_t &value) override;
    virtual bool    st_write_data(Parameter::Address address, uint16_t value) override;

private:
    Response    _relayFrame;
};
