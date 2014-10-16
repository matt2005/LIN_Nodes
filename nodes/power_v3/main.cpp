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

using namespace PowerV3;

void
main(void)
{
    Board::init();
    uint8_t     id = Board::get_mode();

    // check for recovery mode before constructing anything else
    if (Board::get_mode() == 0) {
        Board::panic(Board::kPanicCodeRecovery);
    }

    // init parameters (set to defaults if not valid)
    for (Parameter::Address addr = Generic::kParamConfigBase; addr < Generic::kParamConfigTop; addr++) {
        Parameter p(addr);
        uint8_t encoding = param_encoding(addr);

        if ((encoding != kEncoding_none) && Encoding::invalid(encoding, p.get())) {
            p.set(param_default(addr));
        }
    }

    // construct the slave
    RelaySlave slave(id);
    slave.init();

    // initialise the power switch
    MC17XSF500::configure();

    // enable interrupts
    sei();

    // run the output logic forever
    for (;;) {
        wdt_reset();
        slave.tick();

        // sort out parameter layout - assumes parameters are packed, with all CH1 parameters
        // followed by all CH2 parameters
        const uint8_t channelStride = PowerV3::kParamCH2Assign1 - PowerV3::kParamCH1Assign1;
        const uint8_t assignStride = PowerV3::kParamCH1Assign2 - PowerV3::kParamCH1Assign1;
        const uint16_t assignBase = PowerV3::kParamCH1Assign1;
        const uint16_t pwmBase = PowerV3::kParamCH1PWM1;
        const uint8_t assigns = channelStride / 2;

        // update outputs
        for (unsigned output = 0; output < MC17XSF500::num_channels; output++) {

            uint8_t duty_cycle = 0;

            for (uint8_t assign = 0; assign < assigns; assign++) {
                uint8_t offset = (output * channelStride) + (assign * assignStride);
                uint16_t assigned = Parameter(assignBase + offset).get();

                if (slave.test_relay(assigned)) {
                    uint16_t pwm = Parameter(pwmBase + offset).get();

                    if (pwm > duty_cycle) {
                        duty_cycle = pwm;
                    }
                }
            }

            MC17XSF500::set(output, duty_cycle);
        }
    }
}

void
Parameter::set(uint16_t value) const
{
    switch (address()) {
    case Generic::kParamBootloaderMode:
        if (value == bootloader_magic::kEnterBootloader) {
            Board::enter_bootloader(RelaySlave::node_address(Board::get_mode()),
                                    board_function::kPowerV3);
        }
    }

    if ((address() >> 8) == 0x04) {
        uint8_t index = address() & 0xff;
        eeprom_update_word((uint16_t *)(index * 2), value);
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

    case Generic::kParamBootloaderMode:
        return 0;

    case Generic::kParamFirmwareVersion:
        return RELEASE_VERSION;

    case Generic::kParamFirmwarePageSize:
        return SPM_PAGESIZE;
    }

    if ((address() >> 8) == 0x04) {
        uint8_t index = address() & 0xff;
        return eeprom_read_word((const uint16_t *)(index * 2));
    }

    return 0;
}
