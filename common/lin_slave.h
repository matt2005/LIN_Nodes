///@file lin_slave.h
///
/// LIN driver wrapper for slaves
///

#pragma once

#include <avr/eeprom.h>

#include "lin_protocol.h"
#include "timer.h"

class Slave
{
public:
    typedef uint16_t    Param;

    static const uint8_t maxParam = 32; //< maximum number of supported parameters

#ifdef DEBUG
    volatile uint16_t   nHeader;
    volatile uint16_t   nResponseRx;
    volatile uint16_t   nResponseTx;
#endif

    Slave(LIN::NodeAddress nad);

    /// Called from the transfer-complete ISR
    ///
    void            isrTC();
    
    /// Called from the error ISR
    ///
    void            isrError();

    /// Read a parameter from EEPROM
    ///
    /// @param id               The parameter ID
    /// @return                 The value of the parameter
    ///
    static uint16_t  getParameter(uint8_t id) { return eeprom_read_word((const uint16_t *)0 + id); }

protected:

    enum SleepType {
        kSleepRequested,
        kSleepIdle
    };
    static const uint16_t kIdleTimeout = 4000;  //< 4 seconds

    LIN::NodeAddress _nad;               //< node address 

    /// As the driver to pay attention to the next response that it sees.
    ///
    /// This will usually be the response associated with an interesting
    /// header.
    ///
    /// @param length           The length of the response to be received.
    ///
    void            requestResponse(uint8_t length);
    
    /// Send a response.
    ///
    /// Subclass implementation of headerReceived will call when it wants ti
    /// send a response for an interesting header.
    ///
    /// Responses are always sent in LIN 2.x format, unless the FID from the
    /// header implies 1.x (i.e. SlaveResponse).
    ///
    /// @param frame            The response frame to send.
    /// @param length           The length of the response frame.
    ///
    void            sendResponse(LIN::Frame &frame, uint8_t length);

    /// Prepare a response to the LIN::kSlaveResponse message.
    ///
    /// @param frame            The frame to send in response.
    ///
    void            prepareSlaveResponse(LIN::Frame &frame) 
    {
        _slaveResponse = frame;
        _haveSlaveResponse = true;
    }

    /// Called when a header has been received.
    ///
    /// Must be implemented by the subclass.
    ///
    /// @param fid              The FID from the received header.
    ///
    virtual void    headerReceived(LIN::FID fid);
    
    /// Called when a response requested by requestResponse has been received.
    ///
    /// @param fid              The FID from the header associated with this
    ///                         response.
    /// @param frame            The received response frame.
    ///
    virtual void    responseReceived(LIN::FID fid, LIN::Frame &frame);
    
    /// Called when transmission of a response is completed.
    ///
    /// @param fid              The FID associated with the response that
    ///                         was sent.
    ///
    virtual void    responseSent(LIN::FID fid);

    /// Called when the network is told to sleep.
    ///
    /// @param type             Indicates whether this is a requested or idle
    ///                         timeout sleep request.
    ///
    virtual void    sleepRequested(SleepType type);

    /// Called when a LIN::kMasterRequest response is received that addresses
    /// this node.
    ///
    /// @param frame            The response frame
    ///
    virtual void    masterRequest(LIN::Frame &frame);

    /// Called when a parameter change is received. The default implementation
    /// writes the parameter to EEPROM.
    ///
    /// @param id               The parameter ID
    /// @param value            The new value of the parameter
    ///
    virtual void    setParameter(uint8_t id, Param value);

private:
    Timer           _idleTimer;         //< Bus idle timer
    LIN::FID        _currentFID;        //< the FID from the most recently received header
    LIN::Frame      _frameBuf;          //< working buffer for frame data
    LIN::Frame      _slaveResponse;     //< slave response from previous master request
    bool            _haveSlaveResponse; //< we have a slave response ready to send

    static          Param parameters[];

    static void     idleTimeout();      //< idle timeout callback
};
