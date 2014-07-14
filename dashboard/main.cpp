/*
 * LED matrix test code
 */

#include "graphics.h"
#include "scene.h"
#include "panel.h"
#include "lin.h"
#include "timer.h"
#include "encoder.h"

extern "C" void main(void);

const PaletteEntry palette[16] = {
    PaletteEntry(0,   0,   0).raw(), 	// Black
    PaletteEntry(255, 0,   0).raw(), 	// Red
    PaletteEntry(64,  0,   0).raw(), 	// DimRed
    PaletteEntry(0,   0,   255).raw(), 	// Blue
    PaletteEntry(0,   0,   32).raw(), 	// DimBlue
    PaletteEntry(0,   255, 0).raw(), 	// Green
    PaletteEntry(0,   32,  0).raw(), 	// DimGreen
    PaletteEntry(0,   128, 128).raw(), 	// Cyan
    PaletteEntry(0,   32,  32).raw(), 	// DimCyan
    PaletteEntry(128, 0,   128).raw(), 	// Magenta		avoid - visible components
    PaletteEntry(32,  0,   32).raw(), 	// DimMagenta	avoid - visible components
    PaletteEntry(128, 128, 0).raw(), 	// Yellow		avoid - visible components
    PaletteEntry(32,  32,  0).raw(), 	// DimYellow	avoid - visible components
    PaletteEntry(128, 32,  0).raw(), 	// Amber		avoid - visible components
    PaletteEntry(64,  64,  64).raw(), 	// DimWhite
    PaletteEntry(255, 255, 255).raw(), 	// White
};

LINDev			gLIN(19200);
Panel			gPanel;
Encoder			gEncoder;
extern Scene    gDash;
extern Scene    gPerf;

enum Mode {
    kModeDash,
    kModePerf,
    kModeMax
};

static Scene *
mode_scene(Mode mode)
{
    switch (mode) {
    case kModeDash:
        return &gDash;

    case kModePerf:
        return &gPerf;

    default:
        break;
    }

    return nullptr;
}

static Mode
next_mode(Mode mode)
{
    switch (mode) {
    case kModeDash:
        return kModePerf;

    case kModePerf:
        return kModeDash;

    default:
        break;
    }

    return kModeDash;
}

// XXX 'volatile' required here?
volatile Ticker		refreshTicker(33333);	// 30Hz/33ms

PerfMem perf_mem;

void
main(void)
{
    Mode mode = kModeDash;

    // XXX voodoo - must wait for the first timer interrupt or terrible
    // things happen if we start drawing...
    __asm__ volatile("wfi");

    // spin doing main loop things
    for (;;) {

        // check for encoder events, change mode
        if (mode_scene(mode)->event(gEncoder.event())) {
            mode = next_mode(mode);
        }

        // check for redraw timer expiry
        if (refreshTicker.didTick()) {
            mode_scene(mode)->render();
        }

        // idle and wait for an interrupt
        __asm__ volatile("wfi");
    }

}

extern "C" __attribute__((used, interrupt)) void HardFault_Handler()
{
    for (;;) {
    }
}

