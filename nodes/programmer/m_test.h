/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

///@file m_test.h

#pragma once

#include "timer.h"

#include "menu.h"

namespace Menu
{

class TestMode : public Mode
{
public:
    virtual Mode    *action(Encoder::Event bp) override;

    void            init(uint8_t nad) { _nad = nad; }

private:
    uint8_t         _nad;
    uint8_t         _index;
    Timestamp       _refreshed;

    void            draw();
};

} // namespace Menu

