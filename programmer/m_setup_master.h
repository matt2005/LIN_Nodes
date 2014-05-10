///@file m_setup_master.h

#pragma once

#include "menu.h"

namespace Menu
{

class SetupMasterMode : public Mode
{
public:
    virtual void    enter(Mode *from) override;
    virtual Mode    *action(Encoder::Event bp);

private:
    Mode            *_from;

    uint8_t         _param;

    void            draw();
};

extern SetupMasterMode  modeSetupMaster;

} // namespace Menu

