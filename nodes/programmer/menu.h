/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

///@file menu.h

#pragma once

#include <stdint.h>

#include "print.h"

#include "encoder.h"

namespace Menu
{

/// Called regularly to allow the current mode to update the display, respond
/// to input, etc.
///
void tick();

/// Abstract base class for edit modes
///
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

protected:
    static void     warning(PGM_P message) { msg(PSTR("Warning:"), message); }
    static void     error(PGM_P message) { msg(PSTR("Error:"), message); }

private:
    static void     msg(PGM_P hdr, PGM_P message);
};

} // namespace Menu
