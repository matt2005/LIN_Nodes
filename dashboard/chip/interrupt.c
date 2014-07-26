/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <stdint.h>
#include "lpc111x.h"

typedef void (*func)(void);

void
_badhandler(void)
{
    for (;;) {
        // spin and let the watchdog kill us
    }
}

void NonMaskableInt_Handler(void)       __attribute__((weak, alias("_badhandler")));
void HardFault_Handler(void)            __attribute__((weak, alias("_badhandler")));
void SVCall_Handler(void)               __attribute__((weak, alias("_badhandler")));
void PendSV_Handler(void)               __attribute__((weak, alias("_badhandler")));
void SysTick_Handler(void)              __attribute__((weak, alias("_badhandler")));
void WAKEUP0_Handler(void)              __attribute__((weak, alias("_badhandler")));
void WAKEUP1_Handler(void)              __attribute__((weak, alias("_badhandler")));
void WAKEUP2_Handler(void)              __attribute__((weak, alias("_badhandler")));
void WAKEUP3_Handler(void)              __attribute__((weak, alias("_badhandler")));
void WAKEUP4_Handler(void)              __attribute__((weak, alias("_badhandler")));
void WAKEUP5_Handler(void)              __attribute__((weak, alias("_badhandler")));
void WAKEUP6_Handler(void)              __attribute__((weak, alias("_badhandler")));
void WAKEUP7_Handler(void)              __attribute__((weak, alias("_badhandler")));
void WAKEUP8_Handler(void)              __attribute__((weak, alias("_badhandler")));
void WAKEUP9_Handler(void)              __attribute__((weak, alias("_badhandler")));
void WAKEUP10_Handler(void)             __attribute__((weak, alias("_badhandler")));
void WAKEUP11_Handler(void)             __attribute__((weak, alias("_badhandler")));
void WAKEUP12_Handler(void)             __attribute__((weak, alias("_badhandler")));
void SSP1_Handler(void)                 __attribute__((weak, alias("_badhandler")));
void I2C_Handler(void)                  __attribute__((weak, alias("_badhandler")));
void TIMER_16_0_Handler(void)           __attribute__((weak, alias("_badhandler")));
void TIMER_16_1_Handler(void)           __attribute__((weak, alias("_badhandler")));
void TIMER_32_0_Handler(void)           __attribute__((weak, alias("_badhandler")));
void TIMER_32_1_Handler(void)           __attribute__((weak, alias("_badhandler")));
void SSP0_Handler(void)                 __attribute__((weak, alias("_badhandler")));
void UART_Handler(void)                 __attribute__((weak, alias("_badhandler")));
void ADC_Handler(void)                  __attribute__((weak, alias("_badhandler")));
void WDT_Handler(void)                  __attribute__((weak, alias("_badhandler")));
void BOD_Handler(void)                  __attribute__((weak, alias("_badhandler")));
void EINT3_Handler(void)                __attribute__((weak, alias("_badhandler")));
void EINT2_Handler(void)                __attribute__((weak, alias("_badhandler")));
void EINT1_Handler(void)                __attribute__((weak, alias("_badhandler")));
void EINT0_Handler(void)                __attribute__((weak, alias("_badhandler")));

extern uint8_t _stacktop;
extern void _start(void);

func _vectors[] __attribute__((section(".vectors"))) = {
    [0]                         = (func) &_stacktop,
    [1]                         = (func)_start,
    [16 + NonMaskableInt_IRQn]  = NonMaskableInt_Handler,
    [16 + HardFault_IRQn]       = HardFault_Handler,
    [16 + SVCall_IRQn]          = SVCall_Handler,
    [16 + PendSV_IRQn]          = PendSV_Handler,
    [16 + SysTick_IRQn]         = SysTick_Handler,
    [16 + WAKEUP0_IRQn]         = WAKEUP0_Handler,
    [16 + WAKEUP1_IRQn]         = WAKEUP1_Handler,
    [16 + WAKEUP2_IRQn]         = WAKEUP2_Handler,
    [16 + WAKEUP3_IRQn]         = WAKEUP3_Handler,
    [16 + WAKEUP4_IRQn]         = WAKEUP4_Handler,
    [16 + WAKEUP5_IRQn]         = WAKEUP5_Handler,
    [16 + WAKEUP6_IRQn]         = WAKEUP6_Handler,
    [16 + WAKEUP7_IRQn]         = WAKEUP7_Handler,
    [16 + WAKEUP8_IRQn]         = WAKEUP8_Handler,
    [16 + WAKEUP9_IRQn]         = WAKEUP9_Handler,
    [16 + WAKEUP10_IRQn]        = WAKEUP10_Handler,
    [16 + WAKEUP11_IRQn]        = WAKEUP11_Handler,
    [16 + WAKEUP12_IRQn]        = WAKEUP12_Handler,
    [16 + SSP1_IRQn]            = SSP1_Handler,
    [16 + I2C_IRQn]             = I2C_Handler,
    [16 + TIMER_16_0_IRQn]      = TIMER_16_0_Handler,
    [16 + TIMER_16_1_IRQn]      = TIMER_16_1_Handler,
    [16 + TIMER_32_0_IRQn]      = TIMER_32_0_Handler,
    [16 + TIMER_32_1_IRQn]      = TIMER_32_1_Handler,
    [16 + SSP0_IRQn]            = SSP0_Handler,
    [16 + UART_IRQn]            = UART_Handler,
    [16 + ADC_IRQn]             = ADC_Handler,
    [16 + WDT_IRQn]             = WDT_Handler,
    [16 + BOD_IRQn]             = BOD_Handler,
    [16 + EINT3_IRQn]           = EINT3_Handler,
    [16 + EINT2_IRQn]           = EINT2_Handler,
    [16 + EINT1_IRQn]           = EINT1_Handler,
    [16 + EINT0_IRQn]           = EINT0_Handler,
};
