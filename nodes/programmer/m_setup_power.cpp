
#include "util.h"
#include "board.h"

#include "lin_protocol.h"
#include "protocol.h"
#include "param_power_v1.h"
#include "param_power_v3.h"

#include "m_setup.h"
#include "m_explore.h"
#include "m_edit.h"
#include "slave.h"

namespace Menu
{

SetupPowerMode modeSetupPower;

void
SetupPowerMode::init(uint8_t nad)
{
    _ident = nad - LIN::kNodeAddressPowerBase;

    uint8_t flavour = 0;
    if (gSlave.get_parameter(nad, 0, flavour) == 1) {    // XXX kBoardFunctionID value
        _flavour = kFlavourV1;
    } else {
        _flavour = kFlavourV3;
    }

    _init(nad, Util::strtablen(param_names()) - 1);
}

void
SetupPowerMode::print_title() const
{
    gDisplay.printf(PSTR("Power %1u Setup:"), _ident);
}

Parameter
SetupPowerMode::param() const
{
    return (_flavour == kFlavourV1) ? power_v1Param(_param) : power_v3Param(_param);
}

PGM_P
SetupPowerMode::param_name() const
{
    return Util::strtab(param_names(), _param);
}

PGM_P
SetupPowerMode::param_format() const
{
    return Util::strtab(param_formats(), _param);
}

PGM_P
SetupPowerMode::param_names() const
{
    return (_flavour == kFlavourV1) ? power_v1ParamNames : power_v3ParamNames;
}
PGM_P
SetupPowerMode::param_formats() const
{
    return (_flavour == kFlavourV1) ? power_v1ParamFormats : power_v3ParamFormats;
}

} // namespace Menu
