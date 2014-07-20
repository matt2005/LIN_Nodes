
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
            break;
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
        if (frame.pci() != 3) {
            frame.set_error(LIN::kServiceErrorIncorrectLength);
        } else {
            frame.pci() = 5;
            frame.sid() |= LIN::kServiceIDResponseOffset;
            // select parameter page
            switch (frame.d2()) {
            case 0:
                // page 0 - setup parameters
                frame.d3() = get_param(frame.d1());
                frame.d4() = 0;
                break;
            case 1:
                // page 1 - error counters
                if (frame.d1() < kLINErrorMax) {
                    frame.d3() = errors[frame.d1()];
                    frame.d4() = 0;
                } else {
                    frame.set_error(LIN::kServiceErrorOutOfRange);
                }
                break;
            default:
                frame.set_error(LIN::kServiceErrorOutOfRange);
                break;
            }
        }
        reply = true;
        break;
    }

    case LIN::kServiceIDWriteDataByID: {
        if (frame.pci() != 5) {
            frame.set_error(LIN::kServiceErrorIncorrectLength);
        } else {
            frame.pci() = 3;
            frame.sid() |= LIN::kServiceIDResponseOffset;

            // select parameter page
            switch (frame.d2()) {
            case 0:
                set_param(frame.d1(), frame.d3());      // XXX high bytes ignored
                break;
            default:
                frame.set_error(LIN::kServiceErrorOutOfRange);
                break;
            }
        }
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

        default:
            // send an error
            frame.set_error(LIN::kServiceErrorFunctionNotSupported);
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
