/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include "lpc111x.h"

typedef void (*funcp_t)();

// Symbols from linker script.
extern uint32_t _data_rom;
extern uint32_t _data_start;
extern uint32_t _data_end;
extern uint32_t _bss_start;
extern uint32_t _bss_end;
extern funcp_t _init_array_start;
extern funcp_t _init_array_end;
extern funcp_t _fini_array_start;
extern funcp_t _fini_array_end;

static void clkinit(void);
extern void main(void) __attribute__((noreturn));

void __attribute__((naked))
_start(void)
{
    // Load .data from rom image.
    uint32_t *rp = &_data_rom;
    uint32_t *wp = &_data_start;

    while (wp < &_data_end) {
        *wp++ = *rp++;
    }

    // Clear .bss.
    wp = &_bss_start;

    while (wp < &_bss_end) {
        *wp++ = 0;
    }

    // Fill the stack with 1s
    register unsigned long sp asm("sp");
    wp = (uint32_t *)(sp - 16);

    while (wp >= &_bss_end) {
        *wp-- = 0xffffffff;
    }

    // Bring up the PLL
    clkinit();

    // Call constructors.
    funcp_t *fp = &_init_array_start;

    while (fp < &_init_array_end) {
        (*fp++)();
    }


    // run the app
    main();
}

static void
clkinit(void)
{
    //
    // Configure PLL @ 48MHz
    //

    // swap back to the IRC first...
    SCB_PDRUNCFG &= ~(SCB_PDRUNCFG_SYSOSC_MASK);
    SCB_MAINCLKSEL = SCB_MAINCLKSEL_SOURCE_INTERNALOSC;
    SCB_MAINCLKUEN = SCB_MAINCLKUEN_UPDATE;
    SCB_MAINCLKUEN = SCB_MAINCLKUEN_DISABLE;
    SCB_MAINCLKUEN = SCB_MAINCLKUEN_UPDATE;

    while (!(SCB_MAINCLKUEN & SCB_MAINCLKUEN_UPDATE)) {
        // wait for the bit to self-clear indicating the main clock has switched
    }

    // select the IRC as PLL input
    SCB_PLLCLKSEL = SCB_CLKSEL_SOURCE_INTERNALOSC;  // select internal oscillator
    SCB_PLLCLKUEN = SCB_PLLCLKUEN_UPDATE;
    SCB_PLLCLKUEN = SCB_PLLCLKUEN_DISABLE;
    SCB_PLLCLKUEN = SCB_PLLCLKUEN_UPDATE;

    while (!(SCB_PLLCLKUEN & SCB_PLLCLKUEN_UPDATE)) {
        // wait for the bit to self-clear indicating the PLL input clock has switched
    }

    // turn the PLL off
    SCB_PDRUNCFG |= SCB_PDRUNCFG_SYSPLL_MASK;

    // 4x multiplier, /2 post-divider 12MHz IRC -> 48MHz FCLKOUT
    SCB_PLLCTRL = (SCB_PLLCTRL_MSEL_4 | SCB_PLLCTRL_PSEL_2);

    // turn the PLL back on
    SCB_PDRUNCFG &= ~(SCB_PDRUNCFG_SYSPLL_MASK);

    while (!(SCB_PLLSTAT & SCB_PLLSTAT_LOCK)) {
        // wait for the lock bit to set
    }

    // select the PLL as the main clock
    SCB_MAINCLKSEL = SCB_MAINCLKSEL_SOURCE_SYSPLLCLKOUT;
    SCB_MAINCLKUEN = SCB_MAINCLKUEN_UPDATE;     // toggle high to update clock source
    SCB_MAINCLKUEN = SCB_MAINCLKUEN_DISABLE;    // toggle high to update clock source
    SCB_MAINCLKUEN = SCB_MAINCLKUEN_UPDATE;     // toggle high to update clock source

    while (!(SCB_MAINCLKUEN & SCB_MAINCLKUEN_UPDATE)) {
        // wait for the bit to self-clear indicating the main clock has switched
    }

    //
    // select AHB at 1x core clock
    //
    SCB_SYSAHBCLKDIV = SCB_SYSAHBCLKDIV_DIV1;

}

void __cxa_pure_virtual()
{
    while (1);
}

void *__dso_handle = 0;
