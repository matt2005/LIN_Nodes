#include <string.h>

#include "slave.h"
#include "param_power_v3.h"

RelaySlave::RelaySlave(uint8_t BoardID) :
    Slave(LIN::NodeAddress(LIN::kNodeAddressPowerBase + BoardID - 1))
{
}

void
RelaySlave::st_header_received()
{
    switch (current_FrameID()) {
    case LIN::kFrameIDRelays:
        st_expect_response(8);
        break;

    default:
        Slave::st_header_received();
        break;
    }
}

void
RelaySlave::st_response_received(LIN::Frame &frame)
{
    switch (current_FrameID()) {

    case LIN::kFrameIDRelays:
        relayFrame.copy(frame);
        break;

    default:
        Slave::st_response_received(frame);
        break;
    }
}


uint8_t
RelaySlave::get_param(uint8_t param)
{
    return power_v3Param(param);
}

void
RelaySlave::set_param(uint8_t param, uint8_t value)
{
    power_v3Param(param).set(value);
}
