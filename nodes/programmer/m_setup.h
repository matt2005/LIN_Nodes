///@file m_setup.h

#pragma once

#include "util.h"

#include "menu.h"

namespace Menu
{

class SetupMode : public Mode
{
public:

    virtual Mode    *action(Encoder::Event bp) override;
    static void     init(uint8_t nad);

private:
    enum Flavour : uint8_t {
        kFlavourMaster,
        kFlavourPowerV1,
        kFlavourPowerV3
    };

    static uint8_t  _nad;
    static uint8_t  _param;
    static uint8_t  _value;

    static Flavour  _flavour;
    static uint8_t  _max_param;
    static bool     _editing;

    void            draw();

    static void     print_title();
    static Parameter param();
    static PGM_P    param_names();
    static PGM_P    param_formats();

    static PGM_P    param_name()
    {
        return Util::strtab(param_names(), _param);
    }

    static PGM_P    param_format()
    {
        return Util::strtab(param_formats(), _param);
    }

};

} // namespace Menu

