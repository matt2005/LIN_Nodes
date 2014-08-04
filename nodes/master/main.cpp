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
#include <avr/io.h>
#include <avr/eeprom.h>

#include "board.h"
#include "timer.h"

#include "master.h"
#include "switches.h"
#include "relays.h"

#include "lin_defs.h"

bool __cxa_guard_acquire() { return true; }
void __cxa_guard_release() {}

MasterNode  gMaster;

void
main(void)
{
    Board::init();

    // check for recovery mode before doing anything else
    if (Board::get_mode() != 0) {
        Board::panic(Board::kPanicCodeRecovery);
    }

    // init parameters (set to defaults if not valid)
    for (Parameter::Address addr = 0x0400; ; addr++) {
        Parameter p = Master::parameter(addr);
        if (!p.exists()) {
            break;
        }
        p.init();
    }


    // initialisation
    gMaster.init();         // on v1 boards, must do this before SPI due to !SS being LINCS
    Switches::init();

    // enable interrupts; timers and LIN events will start.
    sei();

    // run the master logic forever
    for (;;) {
        wdt_reset();

#ifdef DEBUG

        if (Board::freemem() < 64) {
            Board::panic(Board::kPanicCodeLowMemory);
        }

#endif

        // scan the switch array
        Switches::scan();

        // Run the relay logic, update our idea of what the vehicle state should be.
        // Results are broadcast routinely anytime the vehicle is awake.
        Relays::tick();

    }
}

void
Parameter::set(uint16_t value) const
{
    switch (address()) {
    case Generic::kParamBootloaderMode:
        if (value == 0x4f42) {        // 'BO'
            Board::enter_bootloader();
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
        return 1;
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
