///@file m_setup_power.h

#pragma once

#include "menu.h"

namespace Menu
{

class SetupPowerMode : public Mode
{
public:
    SetupPowerMode() :
        _node(0),
        _param(0),
        _value(0),
        _editing(false)
    {}

    void            init(uint8_t node) { _node = node; }

    virtual Mode    *action(Encoder::Event bp) override;

private:
    uint8_t         _node;
    uint8_t         _param;
    uint8_t         _value;
    bool            _editing;

    void            draw();
};

extern SetupPowerMode  modeSetupPower;

} // namespace Menu

