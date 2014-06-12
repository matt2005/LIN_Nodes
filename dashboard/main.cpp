/*
 * LED matrix test code
 */

#include "debug.h"
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
PanelV2PIO		PanelDriver;
Panel			gPanel(PanelDriver);

// rotary encoder
Encoder			gEncoder;

//#define COLOUR_TEST

Scene			gDash(gPanel);

#if defined(COLOUR_TEST)

static GlyphIcon	ct_red			(gDash, Position( 0,  0), g_telltale, Red);
static GlyphIcon	ct_dimred		(gDash, Position( 4,  0), g_telltale, DimRed);
static GlyphIcon	ct_green		(gDash, Position( 8,  0), g_telltale, Green);
static GlyphIcon	ct_dimgreen		(gDash, Position(12,  0), g_telltale, DimGreen);
static GlyphIcon	ct_blue			(gDash, Position(16,  0), g_telltale, Blue);
static GlyphIcon	ct_dimblue		(gDash, Position(20,  0), g_telltale, Blue);
static GlyphIcon	ct_cyan			(gDash, Position(24,  0), g_telltale, Cyan);
static GlyphIcon	ct_dimcyan		(gDash, Position(28,  0), g_telltale, DimCyan);

static GlyphIcon	ct_magenta		(gDash, Position( 0,  16), g_telltale, Magenta);
static GlyphIcon	ct_dimmagenta	(gDash, Position( 4,  16), g_telltale, DimMagenta);
static GlyphIcon	ct_yellow		(gDash, Position( 8,  16), g_telltale, Yellow);
static GlyphIcon	ct_dimyellow	(gDash, Position(12,  16), g_telltale, DimYellow);
static GlyphIcon	ct_amber		(gDash, Position(16,  16), g_telltale, Amber);
static GlyphIcon	ct_white		(gDash, Position(20,  16), g_telltale, White);
static GlyphIcon	ct_dimwhite		(gDash, Position(24,  16), g_telltale, DimWhite);



#else
// Dashboard UI
// sorted from front (draws on top) to back

static const char	t_warn_oil[] = "OIL";
static const char	t_warn_temp[] = "TEMP";
static const char	t_warn_link[] = "LINK";

//static GlyphText	text_link(gDash, Position(12, 0), font_Misc_Fixed_Medium_4x6, 4, Red, t_warn_link);
//static GlyphIcon	err_LIN (gDash, Position(12,4), g_denied, Red, gLIN.errorSummary);

static const char	*t_warn = &t_warn_temp[0];
static GlyphText	text_warn(gDash, Position(12, 0), font_Misc_Fixed_Medium_6x9, 4, Red, t_warn);

static GlyphIcon	tt_left_turn   (gDash, Position(1, 0),  g_left_triangle,  Green,  gLIN.ttLeftTurn);
static GlyphIcon	tt_right_turn  (gDash, Position(29, 0), g_right_triangle, Green,  gLIN.ttRightTurn);
static GlyphIcon	tt_high_beam   (gDash, Position(9, 0),  g_telltale,       Blue,   gLIN.ttHighBeam);
static GlyphIcon	tt_low_beam    (gDash, Position(9, 0),  g_telltale,       Green,  gLIN.ttLowBeam);

static GlyphNumber	num_speed(gDash, Position(40, 12), font_Misc_Fixed_Bold_9x15, 2, DimCyan, gLIN.roadSpeed);
static GlyphNumber	num_rpm  (gDash, Position(0, 12), font_Misc_Fixed_Medium_6x10,  4, DimCyan, gLIN.engineRPM);
static GlyphBar		bar_rpm  (gDash, Region(0, 6, 15, 3), GlyphBar::O_HORIZONTAL, 0, 600, DimGreen, gLIN.engineRPM);



#endif

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
			gDash.render();
		}

		// idle and wait for an interrupt
		perf_mainloop.stop();
		__asm__ volatile("wfi");
	}

}

extern "C" __attribute__ ((used,interrupt)) void HardFault_Handler()
{
	for (;;) {
	}
}

