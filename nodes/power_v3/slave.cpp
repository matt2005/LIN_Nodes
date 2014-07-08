#include <string.h>

#include "slave.h"
#include "param_power_v3.h"

RelaySlave::RelaySlave(uint8_t BoardID) :
    Slave(LIN::NodeAddress(LIN::kNADPowerBase + BoardID - 1))
{
}

void
RelaySlave::headerReceived(LIN::FID fid)
{
    switch (fid) {
    case LIN::kFIDRelays:
        requestResponse(8);
        break;

    default:
        Slave::headerReceived(fid);
        break;
    }
}

void
RelaySlave::responseReceived(LIN::FID fid, LIN::Frame &frame)
{
    switch (fid) {

    case LIN::kFIDRelays:
        relayFrame.copy(frame);
        break;

    default:
        Slave::responseReceived(fid, frame);
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
