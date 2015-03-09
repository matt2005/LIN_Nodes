/*
 * Copyright (c) 2012-2015, Mike Smith, <msmith@purgatory.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * o Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

static_assert(((BL_ADDR + sizeof(bl_core_bin)) < Board::kInfoPage), "bootloader core too large");

void
main()
{
    /* definitely no interrupts */
    cli();

    /* board init, but no watchdog */
    Board::early_init();

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
        wdt_reset();

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
    eeprom_update_byte((uint8_t *)Board::kConfigNodeAddress, 0xff);
    eeprom_update_byte((uint8_t *)Board::kConfigFunction, 0xff);
    eeprom_update_word((uint16_t *)Board::kConfigMagic, Board::kBLMagic);

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
    wdt_enable(WDTO_15MS);

    for (;;) ;
}
