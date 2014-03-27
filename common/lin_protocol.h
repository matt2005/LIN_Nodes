///@file lin_protocol.h
///
/// Common LIN protocol definitions.
///
#pragma once

#include <stdint.h>

namespace LIN
{

union Frame
{
    uint8_t b[8];
    uint8_t w[4];
    uint8_t l[2];
};

typedef uint8_t FID;

enum FrameID
{
    kFIDControls        = 1,
    kFIDAuxSwitches     = 2,

    kFIDMasterRequest   = 0x3c,
    kFIDSlaveResponse   = 0x3d
};

} // namespace LIN

