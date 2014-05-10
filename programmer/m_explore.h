///@file m_explore.h

#pragma once

#include "menu.h"

namespace Menu
{

class ExploreMode : public Mode
{
public:
    virtual void    enter(Mode *from) override;
    virtual Mode    *action(Encoder::Event bp);

private:
    uint8_t         _node;
    void            draw();
    bool            searchUp();
    bool            searchDown();
    bool            nodePresent(uint8_t node);
};

extern ExploreMode modeExplore;

} // namespace Menu
