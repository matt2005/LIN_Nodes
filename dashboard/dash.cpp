/*
 * Dashboard mode
 */

#include "graphics.h"
#include "scene.h"
#include "glyphs.h"
#include "fonts.h"
#include "lin.h"

Scene           gDash;

// status text region
static void         gen_status(GlyphText *gt);
static GlyphText    text_status(&gDash, Region(0, 27, 64, 5), font_Misc_Fixed_Medium_4x6, DimCyan, gen_status);

// telltales
static GlyphIcon    tt_left_turn(&gDash, Position(0, 0), g_left_triangle, Green, gLIN.ttLeftTurn);
static GlyphIcon    tt_right_turn(&gDash, Position(60, 0), g_right_triangle, Green, gLIN.ttRightTurn);
static GlyphIcon    tt_high_beam(&gDash, Position(5, 0), g_highbeam, Blue, gLIN.ttHighBeam);
static GlyphIcon    tt_low_beam(&gDash, Position(5, 0), g_lowbeam, Green, gLIN.ttLowBeam);
static GlyphIcon    tt_fog_lights(&gDash, Position(12, 0), g_highbeam, Green, gLIN.ttFogLights);

// techometer
static GlyphBar     bar_rpm(&gDash, Region(20, 21, 23, 5), GlyphBar::O_HORIZONTAL, 0, 600, DimGreen, gLIN.engineRPM);
static GlyphNumber  num_rpm(&gDash, Position(20, 12), font_Misc_Fixed_Medium_6x10, 4, Cyan, gLIN.engineRPM);

// speedometer
static GlyphNumber  num_speed(&gDash, Position(23, 0), font_Misc_Fixed_Bold_9x15, 2, Cyan, gLIN.roadSpeed);

// minor gauges
static GlyphNumberTenths num_volts(&gDash, Position(46, 21), font_Misc_Fixed_Medium_4x6, 3, Cyan, gLIN.batteryVoltage);
static GlyphNumber  num_temp(&gDash, Position(46, 15), font_Misc_Fixed_Medium_4x6, 3, Cyan, gLIN.waterTemperature);
static GlyphNumber  num_press(&gDash, Position(45, 9), font_Misc_Fixed_Medium_4x6, 3, Cyan, gLIN.oilPressure);
static GlyphIcon    ico_legend(&gDash, Position(57, 10), g_legend, DimGreen);

// fuel level indictor
static GlyphIcon    ico_fuel(&gDash, Position(1, 17), g_E, DimRed);
static GlyphBar     bar_fuel(&gDash, Region(0, 9, 5, 17), GlyphBar::O_VERTICAL, 0, 100, DimGreen, gLIN.fuelLevel);

// AFR indicator (placeholder)
static GlyphBar     bar_afr(&gDash, Region(6, 9, 12, 17), GlyphBar::O_VERTICAL, 0, 100, DimGreen, gLIN.airFuelRatio);

static void
gen_status(GlyphText *gt)
{
    if (!gLIN.linkUp) {
        gt->setColour(Red);
        gt->emit_string("LINK DOWN");

    } else {
        gt->setColour(DimCyan);
        gt->emit_string("OK");
    }
}

