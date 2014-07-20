
#include <avr/power.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "softserial.h"
#include "board.h"

#ifdef pinDebugTX

// 57600
#define BIT_DELAY  16

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
  uint8_t tmp=0;

  asm volatile(
    "sbiw    %0, 0x01   \n\t"
    "ldi     %1, 0xFF   \n\t"
    "cpi     %A0, 0xFF  \n\t"
    "cpc     %B0, %1    \n\t"
    "brne    .-10       \n\t"
    : "+r" (delay), "+a" (tmp)
    : "0" (delay)
    );
}

#else
# error No pinDebugTX defined for this board
#endif
