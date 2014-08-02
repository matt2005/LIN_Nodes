/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

///@file lin_protocol.h

///
/// LIN protocol definitions
///
/// Generally conformant with the LIN 2.1 specification where applicable.
///
/// Extensions are tagged NS.
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
    kFrameIDRelays          = 0x01,     // NS
    kFrameIDECUData         = 0x02,     // NS
    kFrameIDProxyRequest    = 0x3b,     // NS
    kFrameIDMasterRequest   = 0x3c,
    kFrameIDSlaveResponse   = 0x3d,
};

//
// LIN node addresses for MasterRequest/SlaveResponse
//
// Addresses aside from sleep/functional/broadcast are NS
//
enum NodeAddress : uint8_t {
    kNodeAddressSleep           = 0,

    kNodeAddressMaster          = 1,    //< always NAD 1
    kNodeAddressPowerBase       = 2,    //< 15 of these (board ID 2-16)
    kNodeAddressECU             = 18,   //< ECU data bridge
    kNodeAddressDashboard       = 19,   //< dash display (not talkative)
    kNodeAddressTester          = 20,   //< test/diagnostic tool

    kNodeAddressMaxAssigned,

    kNodeAddressFunctional      = 126,
    kNodeAddressBroadcast       = 127,
};

//
// LIN service IDs for  MasterRequest/SlaveResponse
//
enum ServiceID : uint8_t {
    kServiceIDReadDataByID      = 0x22,
    kServiceIDWriteDataByID     = 0x2e,
    kServiceIDTesterPresent     = 0x3e,
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
    kServiceGeneralFailure              = 0x72
};

static const uint16_t   kSupplierID = 0xb007;   //< a random-ish number

class Frame
{
public:

    // Generic constructor
    //Frame() {}
    constexpr Frame(uint8_t b0 = 0,
                    uint8_t b1 = 0,
                    uint8_t b2 = 0,
                    uint8_t b3 = 0,
                    uint8_t b4 = 0,
                    uint8_t b5 = 0,
                    uint8_t b6 = 0,
                    uint8_t b7 = 0) :
        _b { b0, b1, b2, b3, b4, b5, b6, b7 }
    {}

    constexpr Frame(const uint8_t *buf) :
        _b { buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7] }
    {}

    void                copy(const Frame &f)
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

    void                copy(const StaticBitarray<64> &array)
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

    void                clear(const Frame &f)
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
    uint8_t &operator[](uint8_t index) { return _b[index]; }
    const uint8_t &operator[](uint8_t index) const { return _b[index]; }

    // standard transport fields for single frames
    uint8_t    &nad()   { return (*this)[0]; }
    uint8_t    &pci()   { return (*this)[1]; }
    uint8_t    &sid()   { return (*this)[2]; }
    uint8_t    &d1()    { return (*this)[3]; }
    uint8_t    &d2()    { return (*this)[4]; }
    uint8_t    &d3()    { return (*this)[5]; }
    uint8_t    &d4()    { return (*this)[6]; }
    uint8_t    &d5()    { return (*this)[7]; }

    // convert to error reply
    void                set_error(ServiceError err)
    {
        pci() = 2;
        d1() = sid();
        sid() = kServiceIDErrorResponse;
        d2() = err;
        d3() = 0xff;
        d4() = 0xff;
        d5() = 0xff;
    }

    // direct buffer access
    uint8_t    *buf() { return &_b[0]; }

private:
    uint8_t _b[8];
};

// NS
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
    bool        test(RelayID relay) const
    {
        uint8_t index = relay / 8;
        uint8_t bit = 1 << (relay & 0x7);
        return (*this)[index] & bit;
    }
};

class Signal
{
public:
    Signal(Frame &frame, uint8_t offset, uint8_t size) :
        _bitarray(frame.buf()),
        _offset(offset),
        _size(size)
    {
    }

    void            clear() { _bitarray.insert(0, _offset, _size); }

    template<typename T>
    void            set(T n) { _bitarray.insert<T>(n, _offset, _size); }

    template<typename T>
    T               get() { return _bitarray.extract<T>(_offset, _size); }

protected:
    Bitarray        _bitarray;
    const uint8_t   _offset;
    const uint8_t   _size;
};

} // namespace LIN


