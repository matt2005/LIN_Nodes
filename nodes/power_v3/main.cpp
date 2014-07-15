
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "board.h"
#include "slave.h"
#include "mc17xsf500.h"

#include "parameter.h"
#include "protocol.h"
#include "param_power_v3.h"

void
main(void)
{
    Board::init();
    uint8_t     id = Board::get_mode();

    // check for recovery mode before constructing anything else
    if (Board::get_mode() == 0) {
        Board::panic(Board::kPanicCodeRecovery);
    }

    // init/default parameters
    power_v3ParamAll(init);

    // construct the slave
    RelaySlave  slave(id);
    slave.init();

    // initialise the power switch
    MC17XSF500::configure();

    // enable interrupts
    sei();

    // run the output logic forever
    for (;;) {
        wdt_reset();

        // sort out parameter layout
        const uint8_t base = kParamCH1Assign1;
        const uint8_t pwm_base = kParamCH1PWM1;
        const uint8_t stride = kParamCH2Assign1 - kParamCH1Assign1;
        const uint8_t assigns = stride / 2;

        // update outputs
        for (unsigned output = 0; output < MC17XSF500::num_channels; output++) {
            uint8_t duty_cycle = 0;
            uint8_t offset = output * stride;

            for (uint8_t assign = 0; assign < assigns; assign++) {
                if (slave.test_relay((LIN::RelayID)power_v3Param(base + offset + assign).get())) {
                    uint8_t d = power_v3Param(pwm_base + offset + assign);

                    if (d > duty_cycle) {
                        duty_cycle = d;
                    }
                }
            }

            MC17XSF500::set(output, duty_cycle);
        }
    }
}

