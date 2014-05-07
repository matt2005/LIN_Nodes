///@file m_test.h

#pragma once

#include "menu.h"

namespace Menu
{

class TestMode : public Mode
{
public:
    virtual void    enter(Mode *from) override;
    virtual Mode    *action(Button bp);
private:
};

extern TestMode  modeTest;

} // namespace Menu

