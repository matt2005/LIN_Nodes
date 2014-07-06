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
    uint8_t         searchUp(uint8_t from);
    uint8_t         searchDown(uint8_t from);
};

extern ExploreMode modeExplore;

} // namespace Menu
