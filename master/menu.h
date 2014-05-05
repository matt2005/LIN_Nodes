///@file menu.h

#pragma once

#include "display.h"
#include "master.h"
#include "switches.h"

class Mode;

class Menu
{
public:
    Menu(Display &disp, Master &master, Switches &switches);

    /// Tickle the menu state machine
    ///
    void            tick();

private:
    Mode            *_mode;     ///< current mode
};

