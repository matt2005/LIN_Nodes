///@file m_explore.h

#pragma once

#include "menu.h"
#include "bitarray.h"
#include "lin_protocol.h"

namespace Menu
{

class ExploreMode : public Mode
{
public:
    virtual Mode    *action(Encoder::Event bp) override;

private:
    static Bitarray<LIN::kNodeAddressMaxAssigned> presentMask;

    uint8_t         _node;
    void            draw();
    static uint8_t  search_up(uint8_t from);
    static uint8_t  search_down(uint8_t from);
};

extern ExploreMode modeExplore;

} // namespace Menu
