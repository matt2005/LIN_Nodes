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
    uint8_t         _param;
    uint8_t         _value;
    bool            _editing;

    void            draw();
};

extern SetupMasterMode  modeSetupMaster;

} // namespace Menu

