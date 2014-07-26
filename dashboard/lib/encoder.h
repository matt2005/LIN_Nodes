/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

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
