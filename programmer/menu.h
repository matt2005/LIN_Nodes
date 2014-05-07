///@file menu.h

#pragma once

#include <stdint.h>

#include "print.h"

namespace Menu
{

enum Button : uint8_t
{
    kButtonNone = 0,
    kButtonUp,
    kButtonDown,
    kButtonSelect
};

void tick();

class Mode
{
public:
    /// Called when the mode is activated.
    ///
    virtual void    enter(Mode *from) = 0;

    /// Called to give the mode CPU cycles
    Mode            *tick() {
        Button bp = kButtonNone; // XXX get button press
        return action(bp);
    }

protected:
    /// Kick the mode state machine.
    ///
    /// @param disp     The display the mode is using.
    /// @param bp       The oldest un-handled button press; may be
    ///                 zero if no button press has been received.
    /// @return         The mode that is current following processing of
    ///                 this call; should return this if no mode change.
    ///
    virtual Mode    *action(Button bp) = 0;

};

} // namespace Menu
