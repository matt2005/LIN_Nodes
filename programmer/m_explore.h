///@file m_explore.h

#pragma once

#include "menu.h"

namespace Menu
{

class ExploreMode : public Mode
{
public:
    virtual Mode    *action(Encoder::Event bp) override;

private:
    uint8_t         _node;
    void            draw();
    bool            searchUp();
    bool            searchDown();
};

extern ExploreMode modeExplore;

} // namespace Menu
