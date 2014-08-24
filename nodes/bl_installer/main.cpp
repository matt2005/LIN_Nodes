/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <avr/boot.h>

#include <board.h>

const PROGMEM
#include "bl_core.h"

#if BL_ADDR % SPM_PAGESIZE != 0
# error BL_ADDR must be a multiple of SPM_PAGESIZE
#endif

extern "C" void main();

static_assert(((BL_ADDR + sizeof(bl_core_bin)) < (FLASHEND - SPM_PAGESIZE)), "bootloader core too large");

void
main()
{
    /* definitely no interrupts */
    cli();

    /* board init, but no watchdog */
    Board::init();
    wdt_disable();

    /* make sure the eeprom isn't still programming... */
    eeprom_busy_wait();

    /*
     * Erase the first page to all 0xff - kills any hooks to the bootloader
     * that it may have inserted while we were being flashed and causes
     * execution to return here if we are reset while updating.
     */
    boot_page_erase(0);
    boot_spm_busy_wait();

    /*
     * Reflash the bootloader and erase to the end of flash.
     * This will kill the program-valid record in the last page of flash,
     * so that we hang up in the bootloader after reset below.
     * Note that FLASHEND is the address of the last byte of flash, not
     * the first address after the end of the flash...
     */
    uint16_t resid = sizeof(bl_core_bin) / 2;
    uint16_t *src = (uint16_t *)&bl_core_bin[0];
    uint16_t dst = BL_ADDR;

    while (dst < FLASHEND) {
        boot_page_erase(dst);
        boot_spm_busy_wait();

        for (uint8_t i = 0; i < SPM_PAGESIZE; i += 2) {
            if (resid > 0) {
                boot_page_fill(dst + i, pgm_read_word(src++));
                resid--;

            } else {
                boot_page_fill(dst + i, 0xffff);
            }
        }

        boot_page_write(dst);
        boot_spm_busy_wait();

        dst += SPM_PAGESIZE;
    }

    /*
     * Set up the EEPROM for forced bootloader entry and unknown board ID/function
     */
    eeprom_update_byte((uint8_t *)E2END - 3, 0xff);
    eeprom_update_byte((uint8_t *)E2END - 2, 0xff);
    eeprom_update_word((uint16_t *)(E2END - 1), 0x4f42);

    /*
     * Redirect the reset vector back to the bootloader.
     */
    boot_page_fill(0, 0xc000 | ((BL_ADDR / 2) - 1));

    for (uint8_t i = 2; i < SPM_PAGESIZE; i += 2) {
        boot_page_fill(i, 0x0000);
    }

    boot_page_write(0);
    boot_spm_busy_wait();

    // re-enable the watchdog and let it reset us
    wdt_enable(WDTO_60MS);

    for (;;) ;
}
