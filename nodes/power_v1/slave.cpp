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

bool
RelaySlave::st_read_data(uint8_t page, uint8_t index, uint16_t &value)
{
    bool result = false;

    switch (page) {
    case kDataPageStatus:
        // XXX should implement this in a somewhat-generic fashion?
        break;

    case kDataPageNodeStatus:
        // XXX need to gather / report status
        break;

    case kDataPageNodeParameters:
        if (index < kPower_v1ParamMax) {
            value = power_v1Param(index);
            result = true;
        }

        break;

    }

    if (!result) {
        result = Slave::st_read_data(page, index, value);
    }

    return result;
}

bool
RelaySlave::st_write_data(uint8_t page, uint8_t index, uint16_t value)
{
    bool result = false;

    switch (page) {
    case kDataPageNodeParameters:
        if (index < kPower_v1ParamMax) {
            power_v1Param(index).set(value & 0xff);
            result = true;
        }

        break;

    default:
        break;
    }

    if (!result) {
        result = Slave::st_write_data(page, index, value);
    }

    return result;
}
