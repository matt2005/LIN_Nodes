#include <string.h>

#include "slave.h"
#include "board.h"
#include "param_power_v1.h"

RelaySlave::RelaySlave(uint8_t BoardID) :
    Slave(LIN::NodeAddress(LIN::kNodeAddressPowerBase + BoardID - 1))
{
}

void
RelaySlave::st_header_received()
{
    switch (current_FrameID()) {
    case LIN::kFrameIDRelays:
        st_expect_response();
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
    if (param == 0) {
        return kBoardFunctionID;
    }

    return power_v1Param(param);
}

void
RelaySlave::set_param(uint8_t param, uint8_t value)
{
    power_v1Param(param) = value;
}
