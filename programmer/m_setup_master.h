///@file m_setup_master.h

#pragma once

#include "menu.h"

namespace Menu
{

class SetupMasterMode : public Mode
{
public:
    SetupMasterMode() :
        _param(0),
        _value(0),
        _editing(false)
    {}

    virtual Mode    *action(Encoder::Event bp) override;

private:
    uint8_t         _param;
    uint8_t         _value;
    bool            _editing;

    void            draw();
};

extern SetupMasterMode  modeSetupMaster;

} // namespace Menu

