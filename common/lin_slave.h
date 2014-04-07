///@file lin_slave.h
///
/// LIN driver wrapper for slaves
///

#pragma once

#include "lin_protocol.h"

class Slave
{
public:
    Slave(LIN::NodeAddress);

    /// Perform one-time LIN initialisation.
    ///
    static void     init();

    /// Called from the transfer-complete ISR
    ///
    void            isrTC();
    
    /// Called from the error ISR
    ///
    void            isrError();

protected:

    LIN::NodeAddress _nad;               //< node address 
    LIN::Frame      _slaveResponse;     //< slave response from previous master request
    bool            _haveSlaveResponse; //< we have a slave response ready to send

    /// Ask the driver to receive a response.
    ///
    /// Subclass implementation of headerReceived will call when an 
    /// interesting header has been seen; responseReceived will be
    /// called if a response is actually received.
    ///
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

    /// Called when the network is told to sleep
    ///
    virtual void    sleepRequested();

private:
    LIN::FID        _currentFID;    //< the FID from the most recently received header
    LIN::Frame      _frameBuf;      //< working buffer for frame data
};
