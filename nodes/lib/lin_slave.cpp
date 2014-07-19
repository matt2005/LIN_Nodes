
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "lin_slave.h"
#include "board.h"

void
Slave::idle_timeout(void *arg)
{
    auto slave = (Slave *)arg;

    slave->sleep_requested(kSleepTypeIdle);
}

Slave::Slave(LIN::NodeAddress nad) :
    _nad(nad),
    _idleTimer(&Slave::idle_timeout, this)
{
    // start the idle timeout
    _idleTimer.set_remaining(kIdleTimeout);
}

void
Slave::st_header_received()
{
    switch (current_FrameID()) {
    case LIN::kFrameIDMasterRequest:
        _sendSlaveResponse = false;     // clear stale slave response
        st_expect_response();
        break;

    case LIN::kFrameIDSlaveResponse:
        if (_sendSlaveResponse) {
            st_send_response(_response);
            _sendSlaveResponse = false;
        }
        break;

    default:
        LINDev::st_header_received();
    }

    // reset the idle timeout
    _idleTimer.set_remaining(kIdleTimeout);
}

void
Slave::st_response_received(LIN::Frame &frame)
{
    switch (current_FrameID()) {
    case LIN::kFrameIDMasterRequest:
        // check for broadcast sleep request
        if (frame.nad() == LIN::kNodeAddressSleep) {
            sleep_requested(kSleepTypeRequested);
        }
        // check for directly addressed or broadcast master request
        if ((frame.nad() == _nad) ||
            (frame.nad() == LIN::kNodeAddressBroadcast)) {
            if (master_request(frame)) {
                slave_response(frame);
            }
        }
        break;

    default:
        LINDev::st_response_received(frame);
        break;
    }
}

void
Slave::sleep_requested(SleepType type)
{
    // default behaviour is to behave as requested
    Board::sleep();
}

bool
Slave::master_request(LIN::Frame &frame)
{
    bool reply = false;
    // ReadByID
    switch (frame.sid()) {

    case LIN::kServiceIDReadDataByID: {
        frame.pci() = 5;
        frame.sid() |= LIN::kServiceIDResponseOffset;
        frame.d3() = get_param(frame.d1());     // XXX high ID byte ignored
        frame.d4() = 0;
        reply = true;
        break;
    }

    case LIN::kServiceIDWriteDataByID: {
        set_param(frame.d1(), frame.d3());      // XXX high bytes ignored
        frame.pci() = 3;
        frame.sid() |= LIN::kServiceIDResponseOffset;
        reply = true;
        break;
    }

    case LIN::kServiceIDReadByID: {
        switch (frame.d1()) {
        case LIN::kReadByIDProductID:
            frame.pci() = 6;
            frame.sid() |= LIN::kServiceIDResponseOffset;
            frame.d1() = LIN::kSupplierID & 0xff;
            frame.d2() = LIN::kSupplierID >> 8;
            frame.d3() = kBoardFunctionID;
            frame.d4() = Board::get_mode();
            frame.d5() = 0;
            reply = true;
            break;

        case LIN::kReadByIDErrorCounters:
            frame.pci() = 6;
            frame.sid() |= LIN::kServiceIDResponseOffset;
            frame.d1() = errors[kErrorLine];
            frame.d2() = errors[kErrorChecksum] + 
                         errors[kErrorParity] + 
                         errors[kErrorFraming] +
                         errors[kErrorSynchronisation];
            frame.d3() = errors[kErrorProtocol];
            frame.d4() = errors[kErrorSlave1];
            frame.d5() = errors[kErrorSlave2];
            reply = true;
            break;

        default:
            // send an error
            frame.pci() = 3;
            frame.sid() = LIN::kServiceIDErrorResponse;
            frame.d1() = LIN::kServiceIDReadByID;
            frame.d2() = LIN::kServiceErrorFunctionNotSupported;
            frame.d3() = 0xff;
            frame.d4() = 0xff;
            frame.d5() = 0xff;
            reply = true;
            break;
        }
        break;
    }
    }

    return reply;
}

uint8_t
Slave::get_param(uint8_t param)
{
    return 0;
}

void
Slave::set_param(uint8_t param, uint8_t value)
{
}
