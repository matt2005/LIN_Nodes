/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

///@file board.h

///
/// Board pin definitions
///

#pragma once

#include "pin.h"
#include "softserial.h"

#if defined (BOARD_BOOTLOADER)
# include "config_bootloader.h"
#elif defined(BOARD_POWER_V1)
# include "config_power_v1.h"
#elif defined(BOARD_POWER_V3)
# include "config_power_v3.h"
#elif defined(BOARD_MASTER_V1)
# include "config_master_v1.h"
#elif defined(BOARD_PROGRAMMER_V1)
# include "config_programmer_v1.h"
#elif defined(BOARD_USB_LINK)
# include "config_usb_link.h"
#else
# error Need to define a board type
#endif

namespace Board
{
/// Generic 'board' initialisation; clock/power setup, watchdog init, etc.
///
void init();

enum PanicCode : uint8_t {
    kPanicCodeRecovery  = 2,
    kPanicCodeSPI       = 3,
    kPanicCodeLIN       = 4,
    kPanicCodeAssert    = 5,
    kPanicCodeLowMemory = 6
};

/// Panic with a status code
///
void panic(uint8_t code) __attribute__((noreturn));

/// Read the board-specific mode configuration switch (if any)
///
uint8_t get_mode();

/// Put the board to sleep
///
void sleep();

/// Delay for a period in milliseconds, keeping the watchdog at bay
///
void ms_delay(uint16_t ms);

/// Delay for a period in microseconds
///
void us_delay(uint16_t us);

/// Check free space between stack and BSS
///
/// @return         The number of free bytes left between the BSS and stack
///
uint16_t freemem();

/// Reboot into the bootloader
///
void enter_bootloader(uint8_t nad, uint8_t function = 0);

/// Reset
///
void reset();

/// Set the LIN CS state
///
static inline void
lin_CS(bool state)
{
    if (state) {
        pinLINCS.set();

    } else {
        pinLINCS.clear();
    }
}

#if defined(DEBUG) && defined(pinDebugTX)
extern Serial debugPort;
# define debug(fmt, args...)    Board::debugPort.printf(PSTR(fmt "\n"), ## args)
# define debugc(c)              Board::debugPort.putc(c)
#else
# define debug(fmt, args...)    do {} while(0)
# define debugc(c)              do {} while(0)
#endif

} //namespace Board
