/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "board.h"
#include "slave.h"
#include "mc17xsf500.h"

#include "lin_defs.h"

static uint8_t  duty_cycles[MC17XSF500::num_channels];
static uint16_t output_status[MC17XSF500::num_channels];    // XXX inefficient use of memory
static uint16_t current[MC17XSF500::num_channels];

using namespace PowerV3;

static void
param_init()
{
    uint8_t options = eeprom_read_byte((uint8_t *)Board::kConfigOptions);
    bool should_default = (options & Board::kOptResetConfig);

    // init parameters (set to defaults if requested or not valid
    for (Parameter::Address addr = Generic::kParamConfigBase; addr < Generic::kParamConfigTop; addr++) {
        Parameter p(addr);
        uint8_t encoding = param_encoding(addr);

        if (should_default || ((encoding != kEncoding_none) && Encoding::invalid(encoding, p.get()))) {
            p.set(param_default(addr));
        }
    }

    if (should_default) {
        eeprom_update_byte((uint8_t *)Board::kConfigOptions, (options & ~Board::kOptResetConfig));
    }
}

uint8_t
get_assignment(uint8_t channel, uint8_t index)
{
    const uint16_t assign_base = PowerV3::kParamCH1Assign1;
    const uint16_t channel_stride = PowerV3::kParamCH2Assign1 - PowerV3::kParamCH1Assign1;
    const uint16_t assign_stride = PowerV3::kParamCH1Assign2 - PowerV3::kParamCH1Assign1;

    return Parameter(assign_base + channel * channel_stride + index * assign_stride).get();
}

uint8_t
get_pwm(uint8_t channel, uint8_t index)
{
    const uint16_t pwm_base = PowerV3::kParamCH1PWM1;
    const uint16_t channel_stride = PowerV3::kParamCH2PWM1 - PowerV3::kParamCH1PWM1;
    const uint16_t pwm_stride = PowerV3::kParamCH1PWM2 - PowerV3::kParamCH1PWM1;

    return Parameter(pwm_base + channel * channel_stride + index * pwm_stride).get();
}

void
main(void)
{
    Board::early_init();
    uint8_t     id = Board::get_mode();

    // check for recovery mode before constructing anything else
    if (id == 0) {
        Board::panic(Board::kPanicCodeRecovery);
    }

    // construct the slave
    param_init();
    RelaySlave slave(id);
    slave.init();

    // initialise the power switch
    MC17XSF500::configure();

    // enable interrupts
    sei();

    // XXX should set up ADC trigger & collection here

    // run the output logic forever
    for (;;) {
        wdt_reset();
        slave.tick();

        // XXX note that this is hardcoded and should really come from lin_defs.h
        const uint8_t assigns = 4;
        static_assert(kParamCH1Assign4, "mismatched number of per-channel assignments");

        // update outputs - always do this to ensure that any lost update is fixed on
        // the next loop iteration
        for (unsigned output = 0; output < MC17XSF500::num_channels; output++) {

            uint8_t duty_cycle = 0;     // highest duty cycle for any active, assigned output
            bool unassigned = true;     // assume nothing assigned

            for (uint8_t assign = 0; assign < assigns; assign++) {

                uint16_t assigned = get_assignment(output, assign);

                if (assigned != v3_output_assignment::kUnassigned) {
                    unassigned = false;

                    if (slave.test_relay(assigned)) {
                        uint8_t pwm = get_pwm(output, assign);

                        if (pwm > duty_cycle) {
                            duty_cycle = pwm;
                        }
                    }
                }
            }

            if (unassigned) {
                duty_cycles[output] = 0;
                output_status[output] = v3_output_status::kOK;
            } else {
                MC17XSF500::set(output, duty_cycle);
                duty_cycles[output] = duty_cycle;

                // XXX need to use time & knowledge of output type here to be more
                //     intelligent about the state of the load...

                auto status = MC17XSF500::get_status(MC17XSF500::kStatusCH1 + output);

                if (status.chX_status.ots != 0) {
                    output_status[output] = v3_output_status::kTemperatureShutdown;
                } else if (status.chX_status.otw != 0) {
                    output_status[output] = v3_output_status::kTemperatureWarning;
                } else if (status.chX_status.oc_status != 0) {
                    output_status[output] = v3_output_status::kOverCurrent;
                } else if (status.chX_status.olf != 0) {
                    output_status[output] = v3_output_status::kOpenLoad;
                } else {
                    output_status[output] = v3_output_status::kOK;
                }
            }
        }
    }
}

void
Parameter::set(uint16_t value) const
{
    switch (address()) {
    case Generic::kParamOperationMode:
        if (value == operation_magic::kEnterBootloader) {
            Board::enter_bootloader(RelaySlave::node_address(Board::get_mode()),
                                    board_function::kPowerV3);
        }
        break;

    case Generic::kParamConfigBase ... Generic::kParamConfigTop:
        eeprom_update_word((uint16_t *)((address() - Generic::kParamConfigBase) * 2), value);
        break;
    }
}

uint16_t
Parameter::get() const
{
    switch (address()) {
    case Generic::kParamProtocolVersion:
        return Master::kNodeProtocolVersion;

    case Generic::kParamBoardFunction:
        return board_function::kPowerV3;

    case Generic::kParamOperationMode:
        return 0;

    case Generic::kParamFirmwareVersion:
        return RELEASE_VERSION;

    case Generic::kParamFirmwarePageSize:
        return SPM_PAGESIZE;

    case kParamDeviceStatus:
        {
            auto status = MC17XSF500::get_status(MC17XSF500::kStatusQuick);

            if (status.quick_status.qsf != 0) {
                // prioritise output overload reporting
                return v3_device_status::kOverload;
            }
            if (status.quick_status.dsf != 0) {
                status = MC17XSF500::get_status(MC17XSF500::kStatusDevice);

                if (status.device_status.uvf != 0) {
                    return v3_device_status::kUndervoltage;
                }
                if (status.device_status.ovf != 0) {
                    return v3_device_status::kOvervoltage;
                }
                // may be over-enthusiastic, will report RCF and CLKF which may
                // not be "failure"
                return v3_device_status::kDeviceFailure;
            }
        }
        return v3_device_status::kOK;

    case kParamCH1Status:
        return output_status[0];
    case kParamCH2Status:
        return output_status[1];
    case kParamCH3Status:
        return output_status[2];
    case kParamCH4Status:
        return output_status[3];
    case kParamCH5Status:
        return output_status[4];

    case kParamCH1DutyCycle:
        return duty_cycles[0];
    case kParamCH2DutyCycle:
        return duty_cycles[1];
    case kParamCH3DutyCycle:
        return duty_cycles[2];
    case kParamCH4DutyCycle:
        return duty_cycles[3];
    case kParamCH5DutyCycle:
        return duty_cycles[4];

    case kParamCH1Current:
        return current[0];
    case kParamCH2Current:
        return current[1];
    case kParamCH3Current:
        return current[2];
    case kParamCH4Current:
        return current[3];
    case kParamCH5Current:
        return current[4];

    case Generic::kParamConfigBase ... Generic::kParamConfigTop:
        return eeprom_read_word((const uint16_t *)((address() - Generic::kParamConfigBase) * 2));
    }

    return 0;
}
