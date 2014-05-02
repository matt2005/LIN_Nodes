///@file lin_protocol.h
///
/// Common LIN protocol definitions.
///
#pragma once

#include <stdint.h>
#include <string.h>

namespace LIN
{

//
// LIN frame IDs
//
typedef uint8_t FID;

enum FrameID : uint8_t
{
    kFIDNone            = 0,
    kFIDRelays          = 1,
    kFIDAuxSwitches     = 2,

    kFIDMasterRequest   = 0x3c,
    kFIDSlaveResponse   = 0x3d,

    kFIDTest            = 0x14,
    kFIDNonsense        = 0xff
};

//
// Switch IDs
//
enum SwitchID : uint8_t
{
    kSWIgnition,
    kSWStart,
    kSWLights,
    kSWLowBeam,
    kSWHighBeam,
    kSWHighBeamToggle,
    kSWFogLight,
    kSWLeftTurn,
    kSWRightTurn,
    kSWBrake,
    kSWReverse,
    kSWDoor,
    kSWInteriorLight,
    kSWHazard,
    kSWDoorUnlock,

    kSWMax,
    kSWNone             = 0xff
};

//
// Relay IDs for Relays
//
enum RelayID : uint8_t
{
    kRelayLightsUp,
    kRelayLightsDown,
    kRelayLowBeam,
    kRelayHighBeam,
    kRelayFogLights,
    kRelayMarkers,
    kRelayCityLights,
    kRelayLeftTurn,
    kRelayRightTurn,
    kRelayBrake,
    kRelayReverse,
    kRelayInteriorLight,

    kRelayMax,
    kRelayNone          = 0xff
};

//
// LIN node addresses for MasterRequest/SlaveResponse
//
enum NodeAddress : uint8_t
{
    kNADSleep           = 0,

    kNADMaster          = 1,    //< always NAD 1
    kNADAuxSwitches     = 2,
    kNADPowerBase       = 3,

    kNADFunctional      = 126,
    kNADBroadcast       = 127,
};

//
// LIN service IDs for  MasterRequest/SlaveResponse
//
enum ServiceID : uint8_t
{
    kSIDReadByID        = 0xb2,
    kSIDDataDump        = 0xb4,

    kSIDResponseOffset  = 0x40
};

//
// Identifier for ReadByID
//
enum ReadByID : uint8_t {
    kRBIProductID       = 0,
    kRBISerialNumber    = 1,
    kRBIErrorCounters   = 32,
};

static const uint16_t   kSupplierID = 0xb007;   //< a random-ish number

class Frame 
{
public:

    // Generic constructor
    //Frame() {}
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

    // ReadByID request factory
    static Frame    makeReadByIDRequest(uint8_t nad, ReadByID flavor)
    {
        Frame f(nad, 2, kSIDReadByID, flavor);

        return f;
    }

    // frame data copier - avoids some ambiguity around volatile
    // frames
    void                copy(const volatile Frame &f) volatile 
    {
        _b[0] = f._b[0];
        _b[1] = f._b[1];
        _b[2] = f._b[2];
        _b[3] = f._b[3];
        _b[4] = f._b[4];
        _b[5] = f._b[5];
        _b[6] = f._b[6];
        _b[7] = f._b[7];
    }

    // field access by index
    volatile uint8_t &operator[](uint8_t index) volatile { return _b[index]; }
    const volatile uint8_t &operator[](uint8_t index) const volatile { return _b[index]; }

    // field access by name
    volatile uint8_t    &nad()  volatile { return _b[kFINAD]; }
    volatile uint8_t    &pci()  volatile { return _b[kFIPCI]; }
    volatile uint8_t    &sid()  volatile { return _b[kFISID]; }
    volatile uint8_t    &d1()   volatile { return _b[kFID1]; }
    volatile uint8_t    &d2()   volatile { return _b[kFID2]; }
    volatile uint8_t    &d3()   volatile { return _b[kFID3]; }
    volatile uint8_t    &d4()   volatile { return _b[kFID4]; }
    volatile uint8_t    &d5()   volatile { return _b[kFID5]; }

    // direct buffer access
    volatile uint8_t    *buf() volatile { return &_b[0]; }

private:
    enum FrameIndex : uint8_t
    {
        kFINAD              = 0,    //< node address
        kFIPCI              = 1,    //< PCI (always zero) and length (low 4 bits)
        kFISID              = 2,    //< service ID
        kFID1               = 3,
        kFID2               = 4,
        kFID3               = 5,
        kFID4               = 6,
        kFID5               = 7,
    };

    uint8_t _b[8];
};

class RelayFrame : public Frame
{
public:
    void        set(RelayID relay) {
        uint8_t index = relay / 8;
        uint8_t bit = 1 << (relay & 0x7);
        (*this)[index] |= bit;
    }
};


} // namespace LIN

