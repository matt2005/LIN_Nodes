///@file m_setup.h

#pragma once

#include "menu.h"

namespace Menu
{

class SetupMode : public Mode
{
public:
    virtual void    enter(Mode *from) override;
    virtual Mode    *action(Encoder::Event bp);
private:
};

extern SetupMode  modeSetup;

} // namespace Menu

