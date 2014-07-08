
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "lin_slave.h"
#include "board.h"

void
Slave::idleTimeout(void *arg)
{
    auto slave = (Slave *)arg;

    slave->sleepRequested(kSleepIdle);
}

Slave::Slave(LIN::NodeAddress nad) :
    _nad(nad),
    _idleTimer(&Slave::idleTimeout, this)
{
    // start the idle timeout
    _idleTimer.setRemaining(kIdleTimeout);
}

void
Slave::headerReceived(LIN::FID fid)
{
    switch (fid) {
    case LIN::kFIDConfigRequest:
    case LIN::kFIDMasterRequest:
        requestResponse(8);
        break;

    case LIN::kFIDConfigResponse:
        configResponse();
        break;

    case LIN::kFIDSlaveResponse:
        if (_sendSlaveResponse) {
            sendResponse(_response, 8);
            _sendSlaveResponse = false;
        }
        break;

    default:
        break;
    }

    // reset the idle timeout
    _idleTimer.setRemaining(kIdleTimeout);
}

void
Slave::responseReceived(LIN::FID fid, LIN::Frame &frame)
{
    switch (fid) {
    case LIN::kFIDConfigRequest:
        configRequest(reinterpret_cast<LIN::ConfigFrame &>(frame));
        break;

    case LIN::kFIDMasterRequest:
        // check for broadcast sleep request
        if (frame.nad() == LIN::kNADSleep) {
            sleepRequested(kSleepRequested);
        }
        // check for directly addressed or broadcast master request
        if ((frame.nad() == _nad) ||
            (frame.nad() == LIN::kNADBroadcast)) {
            masterRequest(frame);
        }
        break;

    default:
        break;
    }
}

void
Slave::responseSent()
{
}

void
Slave::sleepRequested(SleepType type)
{
    // default behaviour is to behave as requested
    Board::sleep();
}

void
Slave::masterRequest(LIN::Frame &frame)
{
    // ReadByID
    if (frame.sid() == LIN::kSIDReadByID) {
        switch (frame.d1()) {
        case LIN::kRBIProductID:
            frame.pci() = 6;
            frame.sid() |= LIN::kSIDResponseOffset;
            frame.d1() = LIN::kSupplierID & 0xff;
            frame.d2() = LIN::kSupplierID >> 8;
            frame.d3() = BOARD_FUNCTION_ID;
            frame.d4() = Board::getMode();
            frame.d5() = 0;
            break;

        case LIN::kRBIErrorCounters:
            frame.pci() = 6;
            frame.sid() |= LIN::kSIDResponseOffset;
            frame.d1() = errors[kErrLine];
            frame.d2() = errors[kErrChecksum] + 
                         errors[kErrParity] + 
                         errors[kErrFraming] +
                         errors[kErrSynchronisation];
            frame.d3() = errors[kErrProtocol];
            frame.d4() = errors[kErrSlave1];
            frame.d5() = errors[kErrSlave2];
            break;

        default:
            // ignore it
            return;
        }

        slaveResponse(frame);
    }
}

void
Slave::configRequest(LIN::ConfigFrame &frame)
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
        //Parameter(frame.param()).set(frame.value());
        return;
    }
}

void
Slave::configResponse()
{
    if (!_sendConfigResponse) {
        return;
    }
    _sendConfigResponse = false;

    LIN::ConfigFrame f;

    f.nad() = _nad;
    f.flavour() = LIN::kCFGetParam;
    f.param() = _configParam;
    f.value() = get_param(_configParam);
    //f.value() = Parameter(_configParam).get();

    sendResponse(f, 8);  
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
