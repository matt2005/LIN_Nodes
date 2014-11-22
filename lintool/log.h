/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */
#pragma once

namespace Log
{
extern unsigned enable;
extern void acquire();
extern void print();
extern void clear();
extern void trace();

class LogWrapper
{
public:
    ~LogWrapper()
    {
        try {
            Log::acquire();

        } catch (...) {
            // eat any exception here...
        }
    }
};

#define LIN_LOG_BLOCK    Log::LogWrapper _lw_##__LINE__;

} // namespace Log
