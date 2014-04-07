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

enum FrameID : uint8_t
{
    kFIDControls        = 1,
    kFIDAuxSwitches     = 2,

    kFIDMasterRequest   = 0x3c,
    kFIDSlaveResponse   = 0x3d
};

enum FrameIndex : uint8_t
{
    kFINAD              = 0,    //< node address
    kFIPCI              = 1,    //< PCI (always zero) and length (low 4 bits)
    kFISID              = 2,    //< service ID (see below)
    kFID1               = 3,
    kFID2               = 4,
    kFID3               = 5,
    kFID4               = 6,
    kFID5               = 7,
};

enum NodeAddress : uint8_t
{
    kNADSleep           = 0,

    kNADMaster          = 1,    //< always NAD 1

    kNADFunctional      = 126,
    kNADBroadcast       = 127,
};

enum ServiceID : uint8_t
{
    kSIDReadByID        = 0xb2,
    kSIDDataDump        = 0xb4,

    kSIDResponseOffset  = 0x40
};



} // namespace LIN

