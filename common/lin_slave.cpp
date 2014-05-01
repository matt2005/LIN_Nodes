
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
}

void
Slave::headerReceived(LIN::FID fid)
{
    switch (fid) {
    case LIN::kFIDMasterRequest:
        requestResponse(8);
        break;

    case LIN::kFIDSlaveResponse:
        if (_slaveResponse.sid() & LIN::kSIDResponseOffset) {
            sendResponse(_slaveResponse, 8);
            _slaveResponse.sid() = 0;
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
        // product ID
        if (frame.d1() == 0) {
                frame.pci() = 6;
                frame.sid() |= LIN::kSIDResponseOffset;
                frame.d1() = LIN::kSupplierID & 0xff;
                frame.d2() = LIN::kSupplierID >> 8;
                frame.d3() = 1;
                frame.d4() = 0;
                frame.d5() = 0;

        // serial number    
        } else if (frame.d1() == 1) {
            frame.pci() = 5;
            frame.sid() |= LIN::kSIDResponseOffset;
            frame.d1() = 0x01;
            frame.d2() = 0x02;
            frame.d3() = 0x03;
            frame.d4() = 0x04;
            frame.d5() = 0xff;
        } else {
            // nothing to do here
            return;
        }
        prepareSlaveResponse(frame);
    }
}
