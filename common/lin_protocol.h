///@file lin_protocol.h
///
/// Common LIN protocol definitions.
///
/// See also protocol.txt, .awk and .h
///
#pragma once

#include <stdint.h>
#include <string.h>

#include "bitarray.h"
#include "protocol.h"

namespace LIN
{

//
// LIN frame IDs
//
enum FrameID : uint8_t {
    kFrameIDNone            = 0x00,
    kFrameIDRelays          = 0x01,
    kFrameIDECUData         = 0x02,
    kFrameIDConfigRequest   = 0x2c,
    kFrameIDConfigResponse  = 0x2d,
    kFrameIDMasterRequest   = 0x3c,
    kFrameIDSlaveResponse   = 0x3d,
    kFrameIDTest            = 0x3f,
};

// XXX Note - it would be preferable to use the ISO 14229 read/write parameter
// by identifier request and have ConfigRequest become a proxied MasterRequest,
// with a unified SlaveResponse in either case, but it's not clear that
// the request can be formatted in SF mode, and in any case ISO 14229-1 is not
// publically available.
//
// It may be possible to reconstruct the packets using:
// http://unifieddiagnosticservices.blogspot.com/2011/12/automotive-diagnostics-services.html
//
// Read request   d1/d2 = identifier
// Read response  d1/d2 = identifier, d3/d4 = value
// Write request  d1/d2 = identifier, d3/d4 = value
// Write response d1/d2 = identifier
//
// With errors in the 'normal' fashion.
//
// Master could issue a 'get config request' frame & cache response, then send
// that response as part of a MasterRequest.
// Master will need to interlock between this & its own requests... how does this
// work if the master is responding to the SlaveResponse? Does it need to be blind
// to its own responses?
//

//
// LIN node addresses for MasterRequest/SlaveResponse
//
enum NodeAddress : uint8_t {
    kNodeAddressSleep           = 0,

    kNodeAddressMaster          = 1,    //< always NAD 1
    kNodeAddressPowerBase       = 2,    //< 15 of these (board ID 1-15)
    kNodeAddressECU             = 18,   //< ECU data bridge
    kNodeAddressDashboard       = 19,   //< dash display (not talkative)
    kNodeAddressTester          = 20,   //< plug-in programmer

    kNodeAddressMaxAssigned,

    kNodeAddressFunctional      = 126,
    kNodeAddressBroadcast       = 127,
};

//
// LIN service IDs for  MasterRequest/SlaveResponse
//
enum ServiceID : uint8_t {
    kServiceReadDataByID        = 0x22,
    kServiceWriteDataByID       = 0x2e,
    kServiceTesterPresent       = 0x3e,
    kServiceIDReadByID          = 0xb2,
    kServiceIDDataDump          = 0xb4,

    kServiceIDResponseOffset    = 0x40,
    kServiceIDErrorResponse     = 0x7f
};

//
// Identifier for ReadByID
//
enum ReadByID : uint8_t {
    kReadByIDProductID          = 0,
    kReadByIDErrorCounters      = 32,
};

//
// Error codes
//
enum ServiceError : uint8_t {
    kServiceErrorFunctionNotSupported   = 0x12,
    kServiceErrorIncorrectLength        = 0x13,
    kServiceErrorConditionsNotCorrect   = 0x22,
    kServiceErrorOutOfRange             = 0x31,
    kServiceErrorAccessDenied           = 0x33,
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

    Frame(const uint8_t *buf)
    {
        _b[0] = buf[0];
        _b[1] = buf[1];
        _b[2] = buf[2];
        _b[3] = buf[3];
        _b[4] = buf[4];
        _b[5] = buf[5];
        _b[6] = buf[6];
        _b[7] = buf[7];
    }

    // ReadByID request factory
    static Frame        makeReadByIDRequest(uint8_t nad, ReadByID flavor)
    {
        Frame f(nad, 2, kServiceIDReadByID, flavor);

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

    void                copy(const Bitarray<64> &array) volatile
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
    volatile uint8_t    &nad()  volatile { return _b[0]; }
    volatile uint8_t    &pci()  volatile { return _b[1]; }
    volatile uint8_t    &sid()  volatile { return _b[2]; }
    volatile uint8_t    &d1()   volatile { return _b[3]; }
    volatile uint8_t    &d2()   volatile { return _b[4]; }
    volatile uint8_t    &d3()   volatile { return _b[5]; }
    volatile uint8_t    &d4()   volatile { return _b[6]; }
    volatile uint8_t    &d5()   volatile { return _b[7]; }

    // direct buffer access
    volatile uint8_t    *buf() volatile { return &_b[0]; }

private:
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
    bool        test(RelayID relay) const volatile
    {
        uint8_t index = relay / 8;
        uint8_t bit = 1 << (relay & 0x7);
        return (*this)[index] & bit;
    }
};

class ECUDataFrame : public Frame
{
public:
    volatile uint8_t    &RPM()                  volatile { return (*this)[0]; } //< engine RPM / 30, 0 = not running
    volatile uint8_t    &oilPressure()          volatile { return (*this)[1]; } //< PSI
    volatile uint8_t    &oilTemperature()       volatile { return (*this)[2]; } //< def. F
    volatile uint8_t    &coolantTemperature()   volatile { return (*this)[3]; } //< deg. F
    volatile uint8_t    &fuelPressure()         volatile { return (*this)[4]; } //< PSI
    volatile uint8_t    &voltage()              volatile { return (*this)[5]; } //< tenths of a volt
    volatile uint8_t    &AFR()                  volatile { return (*this)[6]; } //< ratio tenths, 10.0 : 20.0
    volatile uint8_t    &roadSpeed()            volatile { return (*this)[7]; } //< mph
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

