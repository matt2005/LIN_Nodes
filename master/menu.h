///@file menu.h

#pragma once

#include "display.h"
#include "master.h"

class Mode;

class Menu
{
public:
    Menu(Display &disp, Master &master);

    /// Tickle the menu state machine
    ///
    void            tick();

private:
    Mode            *_mode;     ///< current mode
};

