/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include "mc17xsf500.h"
#include "board.h"

#include "lin_defs.h"

using namespace PowerV3;

namespace MC17XSF500
{

static Status transfer(Command cmd);
static void wait();

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

class CmdOutputControl : public Command
{
public:
    CmdOutputControl()
    {
        address.address = kCtrlOutputControl;
        address.data = 0;
        output_control.on = 0;
        output_control.psf = 0x1f;
    }
};

class CmdControl : public Command
{
public:
    CmdControl(uint8_t channel, bool state)
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

        if (state) {
            chX_control.on = 1;
            chX_control.pwm = 255;

        } else {
            chX_control.on = 0;
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
            switch (Parameter(kParamCH1Type + channel).get()) {
            case v3_output_type::k5AGeneric:
            case v3_output_type::kLED:
            case v3_output_type::kHID:
            case v3_output_type::kLowPowerBulb:
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
        switch (Parameter(kParamCH1Type).get()) {
        case v3_output_type::kLowPowerBulb:
        case v3_output_type::kHighPowerBulb:
        case v3_output_type::kMotor:
            prescaler_1.prs1 = kPRSDiv4;
            break;

        default:
            break;
        }

        switch (Parameter(kParamCH2Type).get()) {
        case v3_output_type::kLowPowerBulb:
        case v3_output_type::kHighPowerBulb:
        case v3_output_type::kMotor:
            prescaler_1.prs2 = kPRSDiv4;
            break;

        default:
            break;
        }

        switch (Parameter(kParamCH3Type).get()) {
        case v3_output_type::kLowPowerBulb:
        case v3_output_type::kHighPowerBulb:
        case v3_output_type::kMotor:
            prescaler_1.prs3 = kPRSDiv4;
            break;

        default:
            break;
        }

        switch (Parameter(kParamCH4Type).get()) {
        case v3_output_type::kLowPowerBulb:
        case v3_output_type::kHighPowerBulb:
        case v3_output_type::kMotor:
            prescaler_1.prs4 = kPRSDiv4;
            break;

        default:
            break;
        }

        switch (Parameter(kParamCH5Type).get()) {
        case v3_output_type::kLowPowerBulb:
        case v3_output_type::kHighPowerBulb:
        case v3_output_type::kMotor:
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
            switch (Parameter(kParamCH1Type + channel).get()) {
            case v3_output_type::kLowPowerBulb:
            case v3_output_type::kHighPowerBulb:
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
            switch (Parameter(kParamCH1Type + channel).get()) {
            case v3_output_type::kLED:
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
    pinSWRSTB.cfg_output();
    pinSWRSTB.clear();

    // configure SPI
    pinMOSI.cfg_output();
    pinMISO.cfg_input_no_pull();
    pinSCK.cfg_output();
    pinCS.set();
    pinCS.cfg_output();

#ifndef DEBUG
    // configure the SYNCB input
    pinSWSYNCB.cfg_input_pullup();
#endif

    // take the device out of reset and wait the undocumented tENBL
    pinSWRSTB.set();
    Board::ms_delay(1);

    // check that it's the device we expect
    Status stat = get_status(kDeviceID);

    if ((stat.device_id.device_type != 1) || (stat.device_id.device_family != 0)) {
        debug("SPI !MC17SXF500 %4x", stat.val);
        Board::panic(Board::kPanicCodeSPI);
    }

    // configure device
    transfer(CmdInit1());
    transfer(CmdInit2());
    transfer(CmdOutputControl());
    transfer(CmdOverCurrentControl1());
    transfer(CmdPrescaler1());
    transfer(CmdOLControl());
    transfer(CmdOLLEDControl());
}

void
set(uint8_t channel, uint8_t state)
{
    Status s = transfer(CmdControl(channel, state));

    if (s.quick_status.dsf != 0) {
        // XXX device status failures (mostly fatal)
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

//debug("TX: %4x", cmd.val);

    // Configure SPI for mode 2 master at 2MHz, MSB first
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << CPHA);

    // clear flags ready for use
    (void)SPSR;
    (void)SPDR;

    // select the slave and ensure we wait tLEAD
    pinCS.clear();
    Board::us_delay(1);

    // send/recv high byte first
    SPDR = cmd.raw[1];
    wait();
    s.raw[1] = SPDR;

    // send/recv low byte next
    SPDR = cmd.raw[0];
    wait();
    s.raw[0] = SPDR;

    // ensure tLAG delay, deselect the slave and wait at least tCS
    Board::us_delay(1);
    pinCS.set();
    Board::us_delay(1);

//debug("RX: %4x", s.val);

    return s;
}

Status
get_status(uint8_t reg)
{

    // send an init_1 command that will cause the *next* command to
    // return the selected register
    (void)transfer(CmdInit1().soa(reg));

    // send a normal init_1 command and return the status previously selected
    Status s = transfer(CmdInit1());

    if ((s.address.reg) != reg) {
        debug("SPI status read %2x got %2x", reg, s.address.reg);
        Board::panic(Board::kPanicCodeSPI);
    }

    return s;
}

static void
wait()
{
    while (!(SPSR & (1 << SPIF))) {
        if (!(SPCR & (1 << MSTR))) {
            debug("SPI !MSTR");
            Board::panic(Board::kPanicCodeSPI);
        }

        if (SPSR & (1 << WCOL)) {
            debug("SPI WCOL");
            Board::panic(Board::kPanicCodeSPI);
        }
    }
}

} // namespace MC17XSF500
