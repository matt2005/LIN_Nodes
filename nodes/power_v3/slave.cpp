#include <string.h>

#include "slave.h"
#include "param_power_v3.h"

RelaySlave::RelaySlave(uint8_t BoardID) :
    Slave(LIN::NodeAddress(LIN::kNodeAddressPowerBase + BoardID - 1))
{
}

void
RelaySlave::header_received(LIN::FrameID fid)
{
    switch (fid) {
    case LIN::kFrameIDRelays:
        expect_response(8);
        break;

    default:
        Slave::header_received(fid);
        break;
    }
}

void
RelaySlave::response_received(LIN::FrameID fid, LIN::Frame &frame)
{
    switch (fid) {

    case LIN::kFrameIDRelays:
        relayFrame.copy(frame);
        break;

    default:
        Slave::response_received(fid, frame);
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
