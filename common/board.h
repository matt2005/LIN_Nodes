///@file board.h
///
/// Board pin definitions
///

#pragma once

#include "pin.h"
#include "parameter.h"
#include "softserial.h"

// V1 relay node
//
#if defined(BOARD_POWER_V1)

# define pinOUT1        portB[0]
# define pinSTATUS1     portB[1]
# define pinOUT2        portB[2]
# define pinSTATUS2     portB[3]
# define pinOUT3        portB[4]
# define pinSTATUS3     portB[5]
# define pinOUT4        portB[6]
# define pinSTATUS4     portA[7]

# define pinMode1       portA[4]
# define pinMode2       portA[2]
# define pinMode4       portA[5]
# define pinMode8       portA[3]

# define pinLINRX       portA[0]
# define pinLINTX       portA[1]
# define pinLINCS       portA[6]

# define pinDebugTX     portA[7]

#define paramRelay1Assign   Parameter(1)
#define paramRelay2Assign   Parameter(2)
#define paramRelay3Assign   Parameter(3)
#define paramRelay4Assign   Parameter(4)

#endif // BOARD_POWER_V1

// Common pin definitions for boards based on the V1 node.
//
#if defined(BOARD_NODE_V1)

# define pinP0          portB[0]
# define pinP1          portB[1]
# define pinP2          portB[2]
# define pinP3          portB[3]
# define pinP4          portB[4]
# define pinP5          portB[5]
# define pinP6          portB[6]
# define pinP7          portA[7]

# define pinMode1       portA[4]
# define pinMode2       portA[2]
# define pinMode4       portA[5]
# define pinMode8       portA[3]

# define pinMOSI        portA[4]
# define pinMISO        portA[2]
# define pinSCK         portA[5]
# define pinCS          portB[1]

# define pinSDA         portB[0]
# define pinSCL         portB[2]

# define pinLINRX       portA[0]
# define pinLINTX       portA[1]
# define pinLINCS       portA[6]

# define pinADC7        portA[7]
# define pinADC8        portB[5]
# define pinADC9        portB[6]

# define pinINT0        portB[6]

# define pinDebugTX     portA[7]

#endif // BOARD_NODE_V1

#ifndef pinLINRX
# error Need to define a board type
#endif

class Board 
{
public:

    /// Perform chip- and board-level initialisation
    ///
    Board();

    enum PanicCode : uint8_t {
        kPanicRecovery  = 2,
        kPanicI2C       = 3,
        kPanicSPI       = 4,
        kPanicLIN       = 5
    };

    /// Panic with a status code
    ///
    static void         panic(uint8_t code) __attribute__((noreturn));

    /// Read the board-specific mode configuration switch (if any)
    ///
    static uint8_t      getMode();

    /// Put the board to sleep
    ///
    static void         sleep();

    /// Delay for a period in milliseconds, keeping the watchdog at bay
    ///
    static void         msDelay(uint16_t ms);

    /// Delay for a period in microseconds
    ///
    static void         usDelay(uint16_t us);

    /// Check free space between stack and BSS
    ///
    /// @return         The number of free bytes left between the BSS and stack
    ///
    static uint16_t     freemem();

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

#if defined(DEBUG) && defined(pinDebugTX)
    /// Debug serial port
    ///
    static Serial       debugPort;

# define debug(fmt, args...)    Board::debugPort.printfP(PSTR(fmt "\n"), ## args)
#else
# define debug(fmt, args)       do {} while(0)
#endif

};
