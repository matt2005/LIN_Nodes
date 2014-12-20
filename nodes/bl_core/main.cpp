/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <avr/boot.h>
#include <util/delay.h>

#include <board.h>

#include <bl_slave.h>

#if BL_ADDR % SPM_PAGESIZE != 0
# error BL_ADDR must be a multiple of SPM_PAGESIZE
#endif

extern "C" void main();

BLSlave slave;

void
main(void)
{
    // definitely no interrupts yet (XXX is this necessary?)
    cli();

    // check and clear force, or check for program invalid
    if (BLSlave::is_bootloader_forced() || !BLSlave::is_program_valid()) {

        // board init, watchdog disabled
        Board::early_init();

        // init the LIN universe
        slave.init();

        // run the slave
        for (;;) {
            wdt_reset();
            slave.tick();
        }
    }

    // jump to the application
    BLSlave::run_program();
}
