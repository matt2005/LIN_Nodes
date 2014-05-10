///@file m_setup_power.h

#pragma once

#include "menu.h"

namespace Menu
{

class SetupPowerMode : public Mode
{
public:
    void            init(uint8_t node) { _node = node; }

    virtual void    enter(Mode *from) override;
    virtual Mode    *action(Encoder::Event bp);

private:
    uint8_t         _node;
    uint8_t         _param;
    uint8_t         _value;
    bool            _editing;

    void            draw();
};

extern SetupPowerMode  modeSetupPower;

} // namespace Menu

