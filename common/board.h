///@file board.h
///
/// Board pin definitions
///

#pragma once

#include "pin.h"

// The V1 power board mostly looks like a V1 node, but it has additional names for
// the hardwired pins.
//
#if defined(BOARD_POWER_V1)

# define BOARD_NODE_V1

# define pinOUT1       pinP0
# define pinSTATUS1    pinP1
# define pinOUT2       pinP2
# define pinSTATUS2    pinP3
# define pinOUT3       pinP4
# define pinSTATUS3    pinP5
# define pinOUT4       pinP6
# define pinSTATUS4    pinP7

#endif // BOARD_POWER_V1

// Common pin definitions for boards based on the V1 node.
//
#if defined(BOARD_NODE_V1)

# define pinP0      portB[0]
# define pinP1      portB[1]
# define pinP2      portB[2]
# define pinP3      portB[3]
# define pinP4      portB[4]
# define pinP5      portB[5]
# define pinP6      portB[6]
# define pinP7      portA[7]

# define pinMode1   portA[4]
# define pinMode2   portA[2]
# define pinMode4   portA[5]
# define pinMode8   portA[3]

# define pinMOSI    portA[4]
# define pinMISO    portA[2]
# define pinSCK     portA[5]

# define pinSDA     portB[0]
# define pinSCL     portB[2]

# define pinLINRX   portA[0]
# define pinLINTX   portA[1]
# define pinLINCS   portA[6]

# define pinADC7    portA[7]
# define pinADC8    portB[5]
# define pinADC9    portB[6]

# define pinINT0    portB[6]

#endif // BOARD_NODE_V1

class Board 
{
public:

    /// Perform chip- and board-level initialisation
    ///
    Board();

    /// Panic with a status code
    ///
    static void         panic(uint8_t code) __attribute__((noreturn));

    /// Read the board-specific mode configuration switch (if any)
    ///
    static uint8_t      getMode();

    /// Put the board to sleep
    ///
    static void         sleep();

    /// Delay for a period, keeping the watchdog at bay
    ///
    static void         delay(uint16_t ms);

    /// Set the LIN CS state
    ///
    static void         linCS(bool state)
    {
        if (state) {
            pinLINCS.set();
        } else {
            pinLINCS.clear();
        }
    }
};
