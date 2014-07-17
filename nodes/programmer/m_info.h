///@file m_info.h

#pragma once

#include "menu.h"

namespace Menu
{

class InfoMode : public Mode
{
public:
    virtual Mode    *action(Encoder::Event bp) override;
private:
};

} // namespace Menu

