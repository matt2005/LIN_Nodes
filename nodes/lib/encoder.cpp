#include <avr/io.h>
#include <avr/interrupt.h>

#include "board.h"
#include "timer.h"

#include "encoder.h"

namespace Encoder
{

static volatile int8_t turnCount;
static volatile uint8_t buttonCount;

static bool buttonState;
static void button_timeout(void *arg);
static Timer buttonDebounce(button_timeout);

static uint8_t encoderAB = 3;       // saved encoder pin state
static int8_t encoderSubstate = 0;  // intermediate states
static const PROGMEM int8_t encoderLUT[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};

void
init()
{
    pinENCA.cfg_input_pullup();
    pinENCB.cfg_input_pullup();
    pinButton.cfg_input_pullup();

    ENC_INT_REG |= ENC_INT_BITS;
    PCICR |= ENC_INT_BIT;
}

Event
event()
{
    if (buttonCount > 0) {
        buttonCount = 0;
        return kEventPress;
    }

    if (turnCount < 0) {
        turnCount = 0;
        return kEventDown;
    }

    if (turnCount > 0) {
        turnCount = 0;
        return kEventUp;
    }

    return kEventNone;
}

void
discard()
{
    turnCount = buttonCount = 0;
}

static void
button_timeout(void *arg)
{
    buttonState = !buttonState;

    if (buttonState) {
        buttonCount++;
    }
}

static void
pcint()
{
    if (pinButton.get() != buttonState) {
        // cancel timer (if any)
        buttonDebounce.set_remaining(0);

    } else {
        // (re)start timer
        buttonDebounce.set_remaining(10);
    }

    // generate encoder LUT index
    encoderAB <<= 2;                    // previous state in high index bits
    encoderAB |= pinENCA.get() ? 2 : 0; // new state
    encoderAB |= pinENCB.get() ? 1 : 0; // ... in low index bits

    // traverse the state table
    encoderSubstate += pgm_read_byte(&encoderLUT[encoderAB & 0x0f]);

    if (encoderSubstate > 3) {          // 4 substates -> one whole state
        turnCount++;
        encoderSubstate = 0;

    } else if (encoderSubstate < -3) {  // 4 substates -> one whole state
        turnCount--;
        encoderSubstate = 0;
    }

}

}

ISR(PCINT0_vect)
{
    Encoder::pcint();
}
