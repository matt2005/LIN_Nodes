
#include "util.h"
#include "board.h"

#include "lin_protocol.h"
#include "protocol.h"
#include "param_master.h"

#include "m_setup.h"
#include "m_explore.h"
#include "m_edit.h"
#include "slave.h"

namespace Menu
{

SetupMasterMode modeSetupMaster;

void
SetupMasterMode::init()
{
    _init(LIN::kNodeAddressMaster, Util::strtablen(masterParamNames) - 1);
}

void
SetupMasterMode::print_title() const
{
    gDisplay.printf(PSTR("Master Setup:"));
}

Parameter
SetupMasterMode::param() const
{
    return masterParam(_param);
}

PGM_P
SetupMasterMode::param_name() const
{
    return Util::strtab(masterParamNames, _param);
}

PGM_P
SetupMasterMode::param_format() const
{
    return Util::strtab(masterParamFormats, _param);
}

} // namespace Menu
