///@file m_explore.h

#pragma once

#include "menu.h"

namespace Menu
{

class ExploreMode : public Mode
{
public:
    virtual void    enter(Mode *from) override;
    virtual Mode    *action(Button bp);

private:
    uint8_t         _node;
    bool            _present;
    void            check();
};

extern ExploreMode modeExplore;

} // namespace Menu
