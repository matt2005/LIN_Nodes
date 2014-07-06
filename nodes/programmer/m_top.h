///@file m_top.h

#pragma once

#include "menu.h"

namespace Menu
{

/// Top-level menu
///
class TopMode : public Mode
{
public:
    virtual Mode    *action(Encoder::Event bp);
private:
    uint8_t         _index;
    void            draw();
};

extern TopMode  modeTop;

} // namespace Menu

