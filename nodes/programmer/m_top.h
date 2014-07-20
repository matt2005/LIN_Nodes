///@file m_top.h

#pragma once

#include "menu.h"
#include "m_explore.h"
#include "m_info.h"
#include "m_test.h"
#include "m_setup.h"
#include "m_edit.h"

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
extern TestMode             modeTest;
extern EditMode             modeEdit;
extern InfoMode             modeInfo;
} // namespace Menu

