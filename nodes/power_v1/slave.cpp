/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <string.h>

#include "slave.h"
#include "board.h"

RelaySlave::RelaySlave(uint8_t BoardID) :
    Slave(PowerV1::kNodeAddress + BoardID - 1)
{
}

void
RelaySlave::st_header_received()
{
    switch (current_FrameID()) {
    case kFrameIDRelays:
        st_expect_response();
        break;

    default:
        Slave::st_header_received();
        break;
    }
}

void
RelaySlave::st_response_received(Response &frame)
{
    switch (current_FrameID()) {

    case kFrameIDRelays:
        _relayFrame = frame;
        break;

    default:
        Slave::st_response_received(frame);
        break;
    }
}

bool
RelaySlave::st_read_data(Parameter::Address address, uint16_t &value)
{
    // Handle node parameters
    if (PowerV1::parameter(address).exists()) {
        value = PowerV1::parameter(address);
        return true;
    }

    // pass to superclass
    return Slave::st_read_data(address, value);
}

bool
RelaySlave::st_write_data(Parameter::Address address, uint16_t value)
{
    Parameter p = PowerV1::parameter(address);
    if (p.exists()) {
        p = value;
        return true;
    }

    return Slave::st_write_data(address, value);
}
