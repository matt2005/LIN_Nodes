#include "mc17xsf500.h"
#include "board.h"

namespace MC17XSF500
{

static Status transfer(Command cmd);
static Status get_status(StatusRegister reg);
static void wait();

// PWM reference clock frequency, can be 25.6 - 102.4kHz, divided by 256 internally
// before selectable /1, /2, /4 per-channel prescaler.
// 25.6kHz gives PWM clocks 25, 50, 100Hz
#define PWM_REFCLK      25600
#define COMPARE_COUNT   (F_CPU / PWM_REFCLK / 2 - 1)

class CmdInit1 : public Command
{
public:
    CmdInit1()
    {
        address.address = kCtrlInit1;
        address.data = 0;
        init_1.soa = kStatusQuick;
        init_1.soa_mode = 0;
        init_1.mux_sel = kMuxOff;
        init_1.sync_en = kSyncOff;
        init_1.wdsel = 1;                   // long watchdog period
    }

    CmdInit1    &soa(uint8_t reg) { init_1.soa = reg; return *this; }
    CmdInit1    &mux_sel(uint8_t sel) { init_1.mux_sel = sel; return *this; }
};

class CmdInit2 : public Command
{
public:
    CmdInit2()
    {
        address.address = kCtrlInit2;
        address.data = 0;
        init_2.otw_sel = 0;         // XXX ?
        init_2.pwm_sync = 0;
        init_2.ochi_od = 0x1f;      // allow inrush currents when load reconnected on all channels
        init_2.no_hid = 0;          // smart HID ballast detection always enabled
        init_2.ochi_transient = 1;  // improved short-circuit protection XXX TESTME
        init_2.ochi_thermal = 0;    // default thermal limits
    }
};

class CmdControl : public Command
{
public:
    CmdControl(uint8_t channel, uint8_t duty_cycle)
    {
        switch (channel) {
        default:
        case 0:
            address.address = kCtrlCH1Control;
            break;

        case 1:
            address.address = kCtrlCH2Control;
            break;

        case 2:
            address.address = kCtrlCH3Control;
            break;

        case 3:
            address.address = kCtrlCH4Control;
            break;

        case 4:
            address.address = kCtrlCH5Control;
            break;
        }

        address.data = 0;

        if (duty_cycle > 0) {
            chX_control.on = 1;

            switch (paramType(channel)) {
            case kChannelHID:
                chX_control.pwm = 255;              // HID cannot be PWM controlled
                break;

            default:
                chX_control.pwm = duty_cycle;       // XXX add pulse skip support
                chX_control.phase_sel = kPhase0;    // XXX might be nice to shift phases to reduce current?
                break;
            }

        } else {
            chX_control.on = 1;
            chX_control.pwm = 0;
        }
    }
};

class CmdOverCurrentControl1 : public Command
{
public:
    CmdOverCurrentControl1()
    {
        address.address = kCtrlOvercurrentControl1;
        address.data = 0;
        over_current_control_1.acm = 0; // XXX might want to enable for low-current loads?

        for (uint8_t channel = 0; channel < num_channels; channel++) {
            switch (paramType(channel)) {
            case kChannel5AGeneric:
            case kChannelLED:
            case kChannelHID:
            case kChannelLowPowerBulb:
                over_current_control_1.oclo |= (1 << channel);
                break;

            default:
                break;
            }
        }
    }
};

class CmdPrescaler1 : public Command
{
public:
    CmdPrescaler1()
    {
        address.address = kCtrlPrescaler1;
        address.data = 0;

        // low-frequency PWM for motors and bulbs
        switch (paramType(0)) {
        case kChannelLowPowerBulb:
        case kChannelHighPowerBulb:
        case kChannelMotor:
            prescaler_1.prs1 = kPRSDiv4;
            break;

        default:
            break;
        }

        switch (paramType(1)) {
        case kChannelLowPowerBulb:
        case kChannelHighPowerBulb:
        case kChannelMotor:
            prescaler_1.prs2 = kPRSDiv4;
            break;

        default:
            break;
        }

        switch (paramType(2)) {
        case kChannelLowPowerBulb:
        case kChannelHighPowerBulb:
        case kChannelMotor:
            prescaler_1.prs3 = kPRSDiv4;
            break;

        default:
            break;
        }

        switch (paramType(3)) {
        case kChannelLowPowerBulb:
        case kChannelHighPowerBulb:
        case kChannelMotor:
            prescaler_1.prs4 = kPRSDiv4;
            break;

        default:
            break;
        }

        switch (paramType(4)) {
        case kChannelLowPowerBulb:
        case kChannelHighPowerBulb:
        case kChannelMotor:
            prescaler_1.prs5 = kPRSDiv4;
            break;

        default:
            break;
        }
    }
};

class CmdOLControl : public Command
{
public:
    CmdOLControl()
    {
        address.address = kCtrlOLControl;
        address.data = 0;

        for (uint8_t channel = 0; channel < num_channels; channel++) {
            switch (paramType(channel)) {
            case kChannelLowPowerBulb:
            case kChannelHighPowerBulb:
                break;

            default:
                // 2ms deglitch to be conservative
                ol_control.olon_dgl = (1 << channel);
                break;
            }
        }
    }

    void    oloff(uint8_t channels) { ol_control.oloff_en = channels; }
};

class CmdOLLEDControl : public Command
{
public:
    CmdOLLEDControl()
    {
        address.address = kCtrlOLLEDControl;
        address.data = 0;

        for (uint8_t channel = 0; channel < num_channels; channel++) {
            switch (paramType(channel)) {
            case kChannelLED:
                olled_control.olled_en |= (1 << channel);
                break;

            default:
                break;
            }
        }
    }

    void    trigger(uint8_t channels) { olled_control.olled_trg = channels; }
};

void
configure()
{
    // put the device into reset first
    pinSWRSTB.cfgOutput();
    pinSWRSTB.clear();

    // configure SPI
    pinMOSI.cfgOutput();
    pinMISO.cfgInputNoPull();
    pinSCK.cfgOutput();
    pinCS.set();
    pinCS.cfgOutput();

    // PWM clock GPIO setup
    pinSWCLK.set();
    pinSWCLK.cfgOutput();

    // PWM clock driven from timer 1
    // Valid range is 25.6kHz - 102.4kHz.
    TCCR1B = 0;                                 // timer stop
    TIMSK1 = 0;                                 // no interrupts
    TCCR1D = 0;                                 // disable all outputs

    TCCR1C = 0;                                 // clear force compare bits
    TCNT1H = 0;                                 // reset counter to zero
    TCNT1L = 0;

    if (ocrSWCLK < 4) {                         // configure desired timer half
        OCR1AH = COMPARE_COUNT >> 8;
        OCR1AL = COMPARE_COUNT & 0xff;
        TCCR1A = _BV(COM1A0);                   // toggle OC output on compare match

    } else {
        OCR1BH = COMPARE_COUNT >> 8;
        OCR1BL = COMPARE_COUNT & 0xff;
        TCCR1A = _BV(COM1B0);                   // toggle OC output on compare match
    }

    TCCR1D = _BV(ocrSWCLK);                     // enable specific PWM output pin
    TCCR1B = _BV(WGM12) | _BV(CS10);            // ... CTC mode, no prescaler, timer starts

#ifndef DEBUG
    // configure the SYNCB input
    pinSWSYNCB.cfgInputPullUp();
#endif

    // take the device out of reset
    pinSWRSTB.set();

    // check that it's the device we expect
    Status stat = get_status(kDeviceID);

    if (stat.device_id.device_type != 1) {
        debug("SPI !MC17SXF500");
        Board::panic(Board::kPanicSPI);
    }

    // configure device
    transfer(CmdInit1());
    transfer(CmdInit2());
    transfer(CmdOverCurrentControl1());
    transfer(CmdPrescaler1());
    transfer(CmdOLControl());
    transfer(CmdOLLEDControl());
}

void
set(uint8_t channel, uint8_t duty_cycle)
{
    Status s = transfer(CmdControl(channel, duty_cycle));

    if (s.quick_status.dsf != 0) {
        // XXX device status failures (mostly fatal
    }

    if (s.quick_status.ovlf != 0) {
        // XXX overload status (log & mitigate)
    }

    if (s.quick_status.olf != 0) {
        // XXX open load status (log)
    }
}

Status
transfer(Command cmd)
{
    Status s;
    static bool toggle;

    // flip the watchdog toggle on each message
    if (toggle) {
        cmd.address.address |= kCtrlWDToggle;
    }

    toggle = !toggle;

    // Configure SPI for mode 2 master at 2MHz, MSB first
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << CPHA);

    // clear flags ready for use
    (void)SPSR;
    (void)SPDR;

    // select the slave
    pinCS.clear();

    // send/recv high byte first
    SPDR = cmd.raw[1];
    wait();
    s.raw[1] = SPDR;

    // send/recv low byte next
    SPDR = cmd.raw[0];
    wait();
    s.raw[0] = SPDR;

    // deselect the slave
    pinCS.set();

    return s;
}

Status
get_status(StatusRegister reg)
{

    // send an init_1 command that will cause the *next* command to
    // return the selected register
    (void)transfer(CmdInit1().soa(reg));

    // send a normal init_1 command and return the status previously selected
    return transfer(CmdInit1());
}

static void
wait()
{
    while (!(SPSR & (1 << SPIF))) {
        if (!(SPCR & (1 << MSTR))) {
            debug("SPI !MSTR");
            Board::panic(Board::kPanicSPI);
        }

        if (SPSR & (1 << WCOL)) {
            debug("SPI WCOL");
            Board::panic(Board::kPanicSPI);
        }
    }
}

} // namespace MC17XSF500
