///@file lin_protocol.h
///
/// Common LIN protocol definitions.
///
#pragma once

#include <stdint.h>
#include <string.h>

namespace LIN
{

typedef uint8_t FID;

enum FrameID : uint8_t
{
    kFIDNone            = 0,
    kFIDControls        = 1,
    kFIDAuxSwitches     = 2,

    kFIDMasterRequest   = 0x3c,
    kFIDSlaveResponse   = 0x3d
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

static const uint16_t   kSupplierID = 0xb007;   //< a random-ish number

class Frame 
{
public:
    Frame(uint8_t b0 = 0,
          uint8_t b1 = 0,
          uint8_t b2 = 0,
          uint8_t b3 = 0,
          uint8_t b4 = 0,
          uint8_t b5 = 0,
          uint8_t b6 = 0,
          uint8_t b7 = 0)
    {
        _b[0] = b0;
        _b[1] = b1;
        _b[2] = b2;
        _b[3] = b3;
        _b[4] = b4;
        _b[5] = b5;
        _b[6] = b6;
        _b[7] = b7;
    }

    uint8_t &operator[](uint8_t index) { return _b[index]; }

    uint8_t     &nad()   { return _b[kFINAD]; }
    uint8_t     &pci()   { return _b[kFIPCI]; }
    uint8_t     &sid()   { return _b[kFISID]; }
    uint8_t     &d1()    { return _b[kFID1]; }
    uint8_t     &d2()    { return _b[kFID2]; }
    uint8_t     &d3()    { return _b[kFID3]; }
    uint8_t     &d4()    { return _b[kFID4]; }
    uint8_t     &d5()    { return _b[kFID5]; }

    uint8_t     *buf()   { return &_b[0]; }

private:
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

    uint8_t _b[8];
};

enum DataDumpOperations : uint8_t 
{
    kDataDumpGetParam   = 10,
    kDataDumpSetParam   = 11,
};

class DataDumpRequest : public Frame
{
public:
    DataDumpRequest(uint8_t nad,
                    uint8_t d1 = 0,
                    uint8_t d2 = 0,
                    uint8_t d3 = 0,
                    uint8_t d4 = 0,
                    uint8_t d5 = 0) :
    Frame(nad, 0x06, kSIDDataDump, d1, d2, d3, d4, d5)
    {}
};

class DataDumpResponse : public Frame
{
public:
    DataDumpResponse(uint8_t nad,
                    uint8_t d1 = 0,
                    uint8_t d2 = 0,
                    uint8_t d3 = 0,
                    uint8_t d4 = 0,
                    uint8_t d5 = 0) :
    Frame(nad, 0x06, kSIDDataDump | kSIDResponseOffset, d1, d2, d3, d4, d5)
    {}
};

} // namespace LIN

