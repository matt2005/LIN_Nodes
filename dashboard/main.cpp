/*
 * LED matrix test code
 */

#include "graphics.h"
#include "scene.h"
#include "panel.h"
#include "glyphs.h"
#include "fonts.h"
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
    PaletteEntry(0,   64,  0).raw(), 	// DimGreen
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

// LIN receiver
LINDev			gLIN(19200);

// Display
Panel			gPanel;

// rotary encoder
Encoder			gEncoder;

// dashboard scene
Scene			gDash(gPanel, "DASH");

// Dashboard UI
// sorted from front (draws on top) to back

static void
test_generator(GlyphText *gt)
{
    static const char   *t_msg = "TEST MESSAGE 12345678";

    for (const char *cp = t_msg; *cp != 0; cp++)
        gt->emit(*cp);
}

static GlyphText	text_info(gDash, Region(0, 27, 64, 5), font_Misc_Fixed_Medium_4x6, Red, test_generator);

static GlyphIcon	tt_left_turn(gDash, Position(0, 0),  g_left_triangle,  Green,  gLIN.ttLeftTurn);
static GlyphIcon	tt_right_turn(gDash, Position(60, 0), g_right_triangle, Green,  gLIN.ttRightTurn);
static GlyphIcon	tt_high_beam(gDash, Position(5, 0),  g_highbeam,       Blue,   gLIN.ttHighBeam);
static GlyphIcon	tt_low_beam(gDash, Position(5, 0),  g_lowbeam,        Green,  gLIN.ttLowBeam);
static GlyphIcon	tt_fog_lights(gDash, Position(12, 0), g_highbeam,       Green,  gLIN.ttFogLights);

static GlyphBar		bar_rpm(gDash, Region(20, 21, 23, 5), GlyphBar::O_HORIZONTAL, 0, 600, DimGreen, gLIN.engineRPM);
static GlyphNumber	num_rpm(gDash, Position(20, 12), font_Misc_Fixed_Medium_6x10,  4, Cyan, gLIN.engineRPM);
static GlyphNumber	num_speed(gDash, Position(23, 0), font_Misc_Fixed_Bold_9x15, 2, Cyan, gLIN.roadSpeed);

static GlyphNumber	num_volts(gDash, Position(46, 21), font_Misc_Fixed_Medium_4x6, 3, DimCyan, gLIN.batteryVoltage);
static GlyphIcon	ico_volts(gDash, Position(59, 23), g_V, DimCyan);
static GlyphNumber	num_temp(gDash, Position(46, 15), font_Misc_Fixed_Medium_4x6, 3, DimCyan, gLIN.waterTemperature);
static GlyphIcon	ico_temp(gDash, Position(58, 15), g_degF, DimCyan);
static GlyphNumber	num_press(gDash, Position(45, 9), font_Misc_Fixed_Medium_4x6, 3, DimCyan, gLIN.oilPressure);
static GlyphIcon	ico_psi(gDash, Position(57, 10), g_psi, DimCyan);

static GlyphIcon    ico_fuel(gDash, Position(1, 17), g_E, DimRed);
static GlyphBar		bar_fuel(gDash, Region(0, 9, 5, 17), GlyphBar::O_VERTICAL, 0, 100, DimGreen, gLIN.fuelLevel);

// Text UI
Scene           gText(gPanel, "TEXT");
static GlyphText    t_text(gText, Region(0, 0, 64, 32), font_Misc_Fixed_Medium_4x6, DimWhite, test_generator);


volatile Ticker		refreshTicker(33333);	// 30Hz

void
main(void)
{
    // main loop load counter
    PerfLoad perf_mainloop("mainloop");

    // XXX voodoo - must wait for the first timer interrupt or terrible
    // things happen if we start drawing...
    __asm__ volatile("wfi");

    //gDash.render();

    //gPanel.clear();
    //gPanel.fill(Red);
    //gPanel.draw(Position(0, 9), Blue);

    // spin doing main loop things
    for (;;) {
        perf_mainloop.start();

        if (refreshTicker.didTick()) {
            gText.render();
//            gDash.render();
        }

        // idle and wait for an interrupt
        perf_mainloop.stop();
        __asm__ volatile("wfi");
    }

}

extern "C" __attribute__((used, interrupt)) void HardFault_Handler()
{
    for (;;) {
    }
}

