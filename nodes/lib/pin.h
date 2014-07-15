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

        void cfg_output()       {          _port.DDR |=  _bit; }
        void cfg_input_no_pull()  { clear(); _port.DDR &= ~_bit; }
        void cfg_input_pullup()  { set();   _port.DDR &= ~_bit; }

        void set()             { _port.PORT |=  _bit; }
        void clear()           { _port.PORT &= ~_bit; }
        void toggle()          { _port.PIN  |=  _bit; }

        bool get() const       { return _port.PIN & _bit; }
        operator bool () const { return get(); }
    };

    constexpr Port(Reg_t &ddr, Reg_t &port, Reg_t &pin) : DDR(ddr), PORT(port), PIN(pin) {}
    constexpr Pin operator[](uint8_t pin) {
        return Pin(*this, pin);
    }
    void set(uint8_t val) { PORT = val; };
};

static Port portA(DDRA, PORTA, PINA);
static Port portB(DDRB, PORTB, PINB);
