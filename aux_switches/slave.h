///@file slave.h

#pragma once

#include "lin_slave.h"

class SwitchSlave : public Slave
{
public:

protected:
    virtual void headerReceived(LIN::FID fid) override;

private:
    static const uint8_t    kNumChannels = 16;

    uint16_t                _switches;

    // Switch configuration parameters are:
    //
    // utdbbbbb
    //
    // Where:
    //  u     indicates the switch is unassigned
    //  t     indicates a tri-stated input (comparator with no pull)
    //  d     indicates a switch-to-power input (not all channels support this)
    //  bbbbb is the bit shift in the kFIDAuxSwitches field.
    //
    static const uint8_t    kConfigUnassigned   = (1 << 7);
    static const uint8_t    kConfigTristate     = (1 << 6);
    static const uint8_t    kConfigPullDown     = (1 << 5);
    static const uint8_t    kConfigChannelMask  = 0x1f;

    uint8_t                 _config[kNumChannels];

    void                    reportSwitches();
};

