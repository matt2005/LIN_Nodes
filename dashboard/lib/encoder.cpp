#include "encoder.h"
#include "lpc111x.h"

static const int8_t encoderLUT[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};

static Encoder *_encoder;

Encoder::Encoder() :
    _debounceTimer(debounceTimeout, this),
    _turnCount(0),
    _buttonCount(0),
    _encoderAB(3),
    _encoderSubstate(0),
    _buttonState(false)
{
    _encoder = this;

    SCB_SYSAHBCLKCTRL |= SCB_SYSAHBCLKCTRL_GPIO | SCB_SYSAHBCLKCTRL_IOCON;

    // encoder A/B
    IOCON_JTAG_TMS_PIO1_0 = IOCON_JTAG_TMS_PIO1_0_FUNC_GPIO |
                            IOCON_JTAG_TMS_PIO1_0_MODE_PULLUP |
                            IOCON_JTAG_TMS_PIO1_0_ADMODE_DIGITAL;       // button
    IOCON_JTAG_TDO_PIO1_1 = IOCON_JTAG_TDO_PIO1_1_FUNC_GPIO |
                            IOCON_JTAG_TDO_PIO1_1_MODE_PULLUP |
                            IOCON_JTAG_TDO_PIO1_1_ADMODE_DIGITAL;       // A
    IOCON_JTAG_nTRST_PIO1_2 = IOCON_JTAG_nTRST_PIO1_2_FUNC_GPIO |
                              IOCON_JTAG_nTRST_PIO1_2_MODE_PULLUP |
                              IOCON_JTAG_nTRST_PIO1_2_ADMODE_DIGITAL;   // B
    GPIO_GPIO1DIR &= ~(GPIO_IO_P0 | GPIO_IO_P1 | GPIO_IO_P2);           // input
    GPIO_GPIO1IS  &= ~(GPIO_IO_P0 | GPIO_IO_P1 | GPIO_IO_P2);           // edge-sensitive
    GPIO_GPIO1IBE |= (GPIO_IO_P0 | GPIO_IO_P1 | GPIO_IO_P2);            // both edges
    GPIO_GPIO1IE  |= (GPIO_IO_P0 | GPIO_IO_P1 | GPIO_IO_P2);            // interrupt enabled


    // enable pin-change interrupt
    NVIC_EnableIRQ(EINT1_IRQn);
}

Encoder::Event
Encoder::event()
{
    if (_buttonCount > 0) {
        _buttonCount--;
        return EVT_BUTTON;
    }

    if (_turnCount < 0) {
        _turnCount++;
        return EVT_DOWN;
    }

    if (_turnCount > 0) {
        _turnCount--;
        return EVT_UP;
    }

    return EVT_NONE;
}

void
Encoder::interrupt()
{
    // clear interrupt states
    GPIO_GPIO1IC = GPIO_IO_P0 | GPIO_IO_P1 | GPIO_IO_P2;

    // look at current pin state
    bool button = !!(GPIO_GPIO1DATA & GPIO_IO_P0);
    bool encA = !!(GPIO_GPIO1DATA & GPIO_IO_P1);
    bool encB = !!(GPIO_GPIO1DATA & GPIO_IO_P2);

    if (button != _buttonState) {
        _debounceTimer.cancel();

    } else {
        _debounceTimer.callAfter(10000);
    }

    // generate encoder LUT index
    _encoderAB <<= 2;                   // previous state in high index bits
    _encoderAB |= encA ? 2 : 0;         // new state
    _encoderAB |= encB ? 1 : 0;         // ... in low index bits

    // traverse the state table
    _encoderSubstate += encoderLUT[_encoderAB & 0x0f];

    if (_encoderSubstate > 3) {         // 4 substates -> one whole state
        _turnCount++;
        _encoderSubstate = 0;

    } else if (_encoderSubstate < -3) { // 4 substates -> one whole state
        _turnCount--;
        _encoderSubstate = 0;
    }

}

void
Encoder::debounceTimeout(void *arg)
{
    reinterpret_cast<Encoder *>(arg)->_debounceTimeout();
}

void
Encoder::_debounceTimeout()
{
    _buttonState = !_buttonState;

    if (_buttonState) {
        _buttonCount++;
    }
}

extern "C" void EINT1_Handler(void);

void EINT1_Handler(void)
{
    _encoder->interrupt();
}
