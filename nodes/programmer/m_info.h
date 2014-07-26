/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

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

