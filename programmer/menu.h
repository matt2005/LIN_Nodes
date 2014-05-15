///@file menu.h

#pragma once

#include <stdint.h>

#include "print.h"

#include "encoder.h"

namespace Menu
{

void tick();

class Mode
{
public:
    /// Called to give the mode CPU cycles
    Mode            *tick();

    /// Kick the mode state machine.
    ///
    /// @param disp     The display the mode is using.
    /// @param bp       The oldest un-handled button press; may be
    ///                 zero if no button press has been received.
    /// @return         The mode that is current following processing of
    ///                 this call; should return this if no mode change.
    ///
    virtual Mode    *action(Encoder::Event bp) = 0;

};

} // namespace Menu
