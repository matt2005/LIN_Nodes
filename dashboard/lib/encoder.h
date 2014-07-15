#pragma once

#include <stdint.h>

#include "timer.h"

class Encoder
{
public:
    Encoder();

    enum Event {
        EVT_NONE,
        EVT_BUTTON,
        EVT_UP,
        EVT_DOWN
    };
    Event               event();

    void                interrupt();

private:
    Timer               _debounceTimer;

    volatile int8_t     _turnCount;
    volatile uint8_t    _buttonCount;
    uint8_t             _encoderAB;
    int8_t              _encoderSubstate;
    bool                _buttonState;

    static void         debounce_timeout(void *arg);
    void                _debounce_timeout();

};
