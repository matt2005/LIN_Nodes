
#include "board.h"

#include "slave.h"

RelaySlave::RelaySlave(uint8_t BoardID) :
    Slave(LIN::NodeAddress(LIN::kNADPowerBase + BoardID - 1)),
    _monitorTimer(monitor, this, 1000),
    _currentRelayState(0),
    _desiredRelayState(0),
    _outputFault(0)
{
    pinOUT1.clear();
    pinOUT1.cfgOutput();
    pinOUT2.clear();
    pinOUT2.cfgOutput();
    pinOUT3.clear();
    pinOUT3.cfgOutput();
    pinOUT4.clear();
    pinOUT4.cfgOutput();

    pinSTATUS1.cfgInputNoPull();
    pinSTATUS2.cfgInputNoPull();
    pinSTATUS3.cfgInputNoPull();
#ifndef DEBUG                       // XXX this is 'magic' knowledge
    pinSTATUS4.cfgInputNoPull();
#endif
}

void
RelaySlave::headerReceived(LIN::FID fid)
{
    switch (fid) {
    case LIN::kFIDRelays:
        requestResponse(8);
        break;

    case LIN::kFIDTest:
        requestResponse(8);
        break;

    default:
        Slave::headerReceived(fid);
        break;
    }
}

void
RelaySlave::responseReceived(LIN::FID fid, LIN::Frame &frame)
{
    switch (fid) {

    case LIN::kFIDRelays:
        _desiredRelayState = 0;

        for (uint8_t i = 0; i < 4; i++) {
            if (testRelayCmd(frame, (LIN::RelayID)Parameter(i + 1).get())) {
                _desiredRelayState |= (1 << i);
            }
        }
        break;

    case LIN::kFIDTest:
        break;

    default:
        Slave::responseReceived(fid, frame);
        break;
    }
}

void
RelaySlave::monitor(void *arg)
{
    auto slave = (RelaySlave *)arg;

    // status low -> open load -> fault
    //
    // Might want to mask this with enabled outputs, as the 
    // detection of open loads using pullups may not be effective, or
    // with assignments so that un-assigned outputs aren't considered
    // faulty.
    //
    if (pinSTATUS1.get()) {
        slave->_outputFault &= ~1;
    } else {
        slave->_outputFault |= 1;
    }
    if (pinSTATUS2.get()) {
        slave->_outputFault &= ~2;
    } else {
        slave->_outputFault |= 2;
    }
    if (pinSTATUS3.get()) {
        slave->_outputFault &= ~4;
    } else {
        slave->_outputFault |= 4;
    }
#ifndef DEBUG
    if (pinSTATUS4.get()) {
        slave->_outputFault &= ~8;
    } else {
        slave->_outputFault |= 8;
    }
#endif

}
