/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

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

protected:
    static uint8_t  _node;
    virtual Mode    *select();

private:
    static StaticBitarray<LIN::kNodeAddressMaxAssigned> _presentMask;

    static void     draw();
    static uint8_t  search_up(uint8_t from);
    static uint8_t  search_down(uint8_t from);
};

class ExploreSetupMode : public ExploreMode
{
protected:
    virtual Mode    *select() override;
};

class ExploreTestMode : public ExploreMode
{
protected:
    virtual Mode    *select() override;
};

} // namespace Menu
