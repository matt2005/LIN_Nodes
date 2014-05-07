///@file m_top.h

#pragma once

#include "menu.h"

namespace Menu
{

class TopMode : public Mode
{
public:
    virtual void    enter(Mode *from) override;
    virtual Mode    *action(Button bp);
private:
    uint8_t         _index;
    void            draw();
};

extern TopMode  modeTop;

} // namespace Menu

