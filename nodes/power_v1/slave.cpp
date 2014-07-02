#include <string.h>

#include "slave.h"

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
