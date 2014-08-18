/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */


#include <avr/power.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "softserial.h"
#include "board.h"

#ifdef pinDebugTX

// 57600
#if F_CPU == 8000000
# define BIT_DELAY  16
#elif F_CPU == 16000000
# define BIT_DELAY  36
#endif

void
Serial::write(uint8_t c)
{
    // do minimal init - we might be called at any time
    clock_prescale_set(clock_div_1);
    pinDebugTX.set();
    pinDebugTX.cfg_output();

    tx(c);
    wdt_reset();
    Board::us_delay(200);
}

void
Serial::tx(uint8_t c)
{
    uint8_t sreg = SREG;
    cli();

    // start bit
    pinDebugTX.clear();
    tuned_delay(BIT_DELAY);

    // shift data bits
    for (uint8_t mask = 1; mask != 0; mask <<= 1) {
        if (c & mask) {
            pinDebugTX.set();

        } else {
            pinDebugTX.clear();
        }

        tuned_delay(BIT_DELAY);
    }

    // stop bit
    pinDebugTX.set();
    tuned_delay(BIT_DELAY);

    SREG = sreg;
}

void
Serial::tuned_delay(uint16_t delay)
{
    uint8_t tmp = 0;

    asm volatile(
        "sbiw    %0, 0x01   \n\t"
        "ldi     %1, 0xFF   \n\t"
        "cpi     %A0, 0xFF  \n\t"
        "cpc     %B0, %1    \n\t"
        "brne    .-10       \n\t"
        : "+r"(delay), "+a"(tmp)
        : "0"(delay)
    );
}

#else
# error No pinDebugTX defined for this board
#endif
