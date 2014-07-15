#pragma once

class Datum
{
public:
    Datum() : _valid(0), _value(0) {}
    Datum(uint16_t initial) : _valid(1), _value(initial) {}

    operator uint16_t () const { return _value; }
    void operator =(uint16_t v) { _value = v; _valid = 1; }

    bool        is_valid() const { return _valid != 0; }
    void        invalidate() { _valid = 0; _value = 0; }

private:
    volatile uint16_t   _valid: 1;
    volatile uint16_t   _value: 15;
};

static const Datum dTrue(true);
static const Datum dFalse(false);
static const Datum dZero(0);
