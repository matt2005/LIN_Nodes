/*
 * Panel driver.
 */

#include "panel.h"

extern Panel gPanel;

/*
 * LED-on times in microseconds for 1LSB at each of the possible brightness levels.
 *
 * Note that it takes ~50us to clock in a row pair, during which time the LEDs are off.
 * Minimum actual LED on time is dependent on interrupt latency; probably several microseconds.
 */

Panel::Panel(PanelOut &driver) :
	_timer(&Panel::tick, this),
	_driver(driver),
	_phase(0),
	_dim_level(0),
	_perf_line_update("line update"),
	_load("panel")
{
	_timer.callAfter(5000);		// wait a few milliseconds before we start
}

void
Panel::fill(Colour colour)
{
	for (unsigned row = 0; row < _buffer.rows(); row++) {
		for (unsigned column = 0; column < _buffer.columns(); column++) {

			_buffer.subCell(Position(column, row)).set(colour);
		}
	}
}

/*
 * Notes on panel updates.
 *
 * A complete panel update should be scheduled to take ~16ms. Each update is 
 * _depth time slots for each of the _rows/2 row pairs, with the time slot sizing in powers
 * of 2 (XXX tbd scaling) matching the significance of each bit.
 *
 * Each time slot gets two ticks at brightness levels below max; one at the start
 * to turn the LEDs on, and one interior to the slot to turn it off. At max brightness
 * the slots simply run together.
 *
 * The phase is encoded as:
 *
 * 6543210
 * SSSRRRT
 *
 * Where T is 0 for the on toggle and 1 for the off toggle, RRR is the 3-bit current
 * row number, and SSS is the slot number from 0 .. _depth - 1.
 *
 */
void
Panel::tick(void *arg)
{
	reinterpret_cast<Panel *>(arg)->_tick();
}

void
Panel::_tick()
{
	_load.start();
	if (_phase_is_dimming()) {

		_driver.line_off();

	} else {
		_perf_line_update.start();

		_driver.line_update(_phase_row(), _phase_slot(), _buffer);

		_perf_line_update.stop();
	}

	/* advance to the next phase */
	Timer::Interval interval = _phase_advance();

	/* schedule the next tick */
	_timer.callAfter(interval);
	_load.stop();
}

Timer::Interval
Panel::_phase_advance()
{
	unsigned bit_period = _max_brightness << _phase_slot();
	unsigned on_time = bit_period >> _dim_level;
	Timer::Interval interval;

	/* interval to next tick is based on current phase & brightness */
	if (_phase_is_dimming()) {
		/* dimming phase - consume the remainder of the slot time */
		interval = bit_period - on_time;
	} else {
		/* display phase - time based on brightness */
		interval = on_time;
	}

	/* at max brightness, skip the dimming phase */
	_phase += (_dim_level > 0) ? 1 : 2;

	/* at phase wrap, we are in either the display or dimming phase of the last row */
	if (_phase >= _max_phase) {
		_phase = 0;
	}

	return interval;
}
