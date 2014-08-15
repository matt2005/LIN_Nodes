/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

///@file m_top.h

#pragma once

#include "menu.h"
#include "m_explore.h"
#include "m_info.h"
#include "m_setup.h"

namespace Menu
{

/// Top-level menu
///
class TopMode : public Mode
{
public:
    virtual Mode    *action(Encoder::Event bp) override;
private:
    uint8_t         _index;
    void            draw();
};

extern TopMode              modeTop;
extern ExploreSetupMode     modeExploreSetup;
extern SetupMode            modeSetup;
extern ExploreTestMode      modeExploreTest;
extern InfoMode             modeInfo;
} // namespace Menu

