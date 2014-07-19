
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
    case LIN::kFrameIDConfigRequest:
        _sendConfigResponse = false;    // clear stale config response
        st_expect_response(8);
        break;

    case LIN::kFrameIDMasterRequest:
        _sendSlaveResponse = false;     // clear stale slave response
        st_expect_response(8);
        break;

    case LIN::kFrameIDConfigResponse:
        config_response();
        break;

    case LIN::kFrameIDSlaveResponse:
        if (_sendSlaveResponse) {
            st_send_response(_response, 8);
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
    case LIN::kFrameIDConfigRequest:
        config_request(reinterpret_cast<LIN::ConfigFrame &>(frame));
        break;

    case LIN::kFrameIDMasterRequest:
        // check for broadcast sleep request
        if (frame.nad() == LIN::kNodeAddressSleep) {
            sleep_requested(kSleepTypeRequested);
        }
        // check for directly addressed or broadcast master request
        if ((frame.nad() == _nad) ||
            (frame.nad() == LIN::kNodeAddressBroadcast)) {
            master_request(frame);
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

void
Slave::master_request(LIN::Frame &frame)
{
    // ReadByID
    if (frame.sid() == LIN::kServiceIDReadByID) {
        switch (frame.d1()) {
        case LIN::kReadByIDProductID:
            frame.pci() = 6;
            frame.sid() |= LIN::kServiceIDResponseOffset;
            frame.d1() = LIN::kSupplierID & 0xff;
            frame.d2() = LIN::kSupplierID >> 8;
            frame.d3() = kBoardFunctionID;
            frame.d4() = Board::get_mode();
            frame.d5() = 0;
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
            break;
        }

        slave_response(frame);
    }
}

void
Slave::config_request(LIN::ConfigFrame &frame)
{
    if (frame.nad() != _nad) {
        return;
    }
    if (frame.flavour() == LIN::kCFGetParam) {
        _configParam = frame.param();
        _sendConfigResponse = true;
        return;
    }
    if (frame.flavour() == LIN::kCFSetParam) {
        set_param(frame.param(), frame.value());
        return;
    }
}

void
Slave::config_response()
{
    if (!_sendConfigResponse) {
        return;
    }
    _sendConfigResponse = false;

    LIN::ConfigFrame f;

    f.nad() = _nad;
    f.flavour() = LIN::kCFGetParam;
    f.param() = _configParam;
    if (_configParam == 0) {
        f.value() = kBoardFunctionID;
    } else {
        f.value() = get_param(_configParam);
    }

    st_send_response(f, 8);  
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
