
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
    _ident = nad - LIN::kNADPowerBase;

    uint8_t flavour = 0;
    if (gSlave.getParameter(nad, 0, flavour) == 1) {    // XXX BOARD_FUNCTION_ID value
        _flavour = kFlavourV1;
    } else {
        _flavour = kFlavourV3;
    }

    _init(nad, Util::strtablen(paramNames()) - 1);
}

void
SetupPowerMode::printTitle() const
{
    gDisplay.printf(PSTR("Power %1u Setup:"), _ident);
}

Parameter
SetupPowerMode::param() const
{
    return (_flavour == kFlavourV1) ? power_v1Param(_param) : power_v3Param(_param);
}

PGM_P
SetupPowerMode::paramName() const
{
    return Util::strtab(paramNames(), _param);
}

PGM_P
SetupPowerMode::paramFormat() const
{
    return Util::strtab(paramFormats(), _param);
}

PGM_P
SetupPowerMode::paramNames() const
{
    return (_flavour == kFlavourV1) ? power_v1ParamNames : power_v3ParamNames;
}
PGM_P
SetupPowerMode::paramFormats() const
{
    return (_flavour == kFlavourV1) ? power_v1ParamFormats : power_v3ParamFormats;
}

} // namespace Menu
