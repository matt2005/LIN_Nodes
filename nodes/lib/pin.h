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

///@file pin.h

///
/// A simple GPIO driver for the AVR, modelled on the laks GPIO driver.
///

#pragma once

#include <avr/io.h>

class Port
{
public:
    typedef volatile uint8_t Reg_t;

    Reg_t               &DDR;
    Reg_t               &PORT;
    Reg_t               &PIN;

    class Pin
    {
    private:
        const Port      &_port;
        uint8_t         _bit;

    public:
        constexpr Pin(const Port &port, uint8_t pin) : _port(port), _bit(1 << pin) {}

        void cfg_output()        {          _port.DDR |=  _bit; }
        void cfg_input_no_pull() { clear(); _port.DDR &= ~_bit; }
        void cfg_input_pullup()  { set();   _port.DDR &= ~_bit; }

        void set()             { _port.PORT |=  _bit; }
        void clear()           { _port.PORT &= ~_bit; }
        void toggle()          { _port.PIN  |=  _bit; }

        bool get() const       { return _port.PIN & _bit; }
        operator bool () const { return get(); }
    };

    constexpr Port(Reg_t &ddr, Reg_t &port, Reg_t &pin) : DDR(ddr), PORT(port), PIN(pin) {}
    constexpr Pin operator[](uint8_t pin)
    {
        return Pin(*this, pin);
    }
    void set(uint8_t val) { PORT = val; };
};

static Port portA(DDRA, PORTA, PINA);
static Port portB(DDRB, PORTB, PINB);
