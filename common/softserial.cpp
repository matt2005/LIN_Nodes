
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "softserial.h"
#include "board.h"

#ifdef pinDebugTX

// 57600
#define BIT_DELAY  17

Serial::Serial()
{
    pinDebugTX.set();
    pinDebugTX.cfgOutput();
}

void
Serial::_write(uint8_t c)
{
    tx(c);
    wdt_reset();
    Board::usDelay(200);
}

void
Serial::tx(uint8_t c)
{
    uint8_t sreg = SREG;
    cli();

    // start bit
    pinDebugTX.clear();
    tunedDelay(BIT_DELAY);

    // shift data bits
    for (uint8_t mask = 1; mask != 0; mask <<= 1) {
        if (c & mask) {
            pinDebugTX.set();
        } else {
            pinDebugTX.clear();
        }
        tunedDelay(BIT_DELAY);
    }

    // stop bit
    pinDebugTX.set();
    tunedDelay(BIT_DELAY);

    SREG = sreg;
}

void
Serial::tunedDelay(uint16_t delay)
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
