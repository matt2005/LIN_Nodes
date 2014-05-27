///@file lin_protocol.h
///
/// Common LIN protocol definitions.
///
/// See also protocol.txt, .awk and .h
///
#pragma once

#include <stdint.h>
#include <string.h>

#include "util.h"
#include "protocol.h"

namespace LIN
{

//
// LIN frame IDs
//
typedef uint8_t FID;

//
// LIN node addresses for MasterRequest/SlaveResponse
//
enum NodeAddress : uint8_t
{
    kNADSleep           = 0,

    kNADMaster          = 1,    //< always NAD 1
    kNADPowerBase       = 2,    //< 16 of these (board ID 0-15)

    kNADProgrammer      = 20,  //< plug-in programmer

    kNADMaxAssigned,

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

    void                copy(const Util::Bitarray<64> &array) volatile
    {
        _b[0] = array[0];
        _b[1] = array[1];
        _b[2] = array[2];
        _b[3] = array[3];
        _b[4] = array[4];
        _b[5] = array[5];
        _b[6] = array[6];
        _b[7] = array[7];        
    }

    void                clear(const volatile Frame &f) volatile 
    {
        _b[0] = 0;
        _b[1] = 0;
        _b[2] = 0;
        _b[3] = 0;
        _b[4] = 0;
        _b[5] = 0;
        _b[6] = 0;
        _b[7] = 0;
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
    void        set(RelayID relay) 
    {
        uint8_t index = relay / 8;
        uint8_t bit = 1 << (relay & 0x7);
        (*this)[index] |= bit;
    }
    void        clear(RelayID relay) 
    {
        uint8_t index = relay / 8;
        uint8_t bit = 1 << (relay & 0x7);
        (*this)[index] &= ~bit;
    }
    bool        test(RelayID relay) volatile 
    {
        uint8_t index = relay / 8;
        uint8_t bit = 1 << (relay & 0x7);
        return (*this)[index] & bit;
    }
};

enum ConfigFlavour : uint8_t {
    kCFGetParam,
    kCFSetParam,
    kCFNop = 0xff
};

class ConfigFrame : public Frame
{
public:

    volatile uint8_t    &nad()     volatile { return (*this)[0]; }
    volatile uint8_t    &flavour() volatile { return (*this)[1]; }
    volatile uint8_t    &param()   volatile { return (*this)[2]; }
    volatile uint8_t    &value()   volatile { return (*this)[3]; }

};

} // namespace LIN

