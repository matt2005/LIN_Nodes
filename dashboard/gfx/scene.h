#pragma once

#include "graphics.h"
#include "panel.h"
#include "perf.h"
#include "encoder.h"

/* a collection of glyphs that are rendered and drawn to a panel */
class Scene
{
public:
	Scene(Panel &p);

	/**
	 * Add the glyph to the scene glyph stack; new glyphs always stack on top.
	 */
	void		addGlyph(Glyph *g);

	/**
	 * Render the scene.
	 */
	virtual void	render();

	/**
	 * Send an event to the scene.
	 *
	 * Returns true if control should pass to the next scene.
	 */
	virtual bool	event(Encoder::Event evt);

	/**
	 * Place a pixel in the scene.
	 */
	void		draw(Position p, Colour colour)
	{
		// XXX remove clipping test for performance?
		if (_clip(p) && _current_framebuffer)
			_current_framebuffer->draw(p, colour);
	}

	/**
	 * Fill a portion of the scene.
	 */
	void		fill(Region r, Colour colour);

	/**
	 * Return the draw position for a centered item of the given dimensions.
	 */
	Position	centeredPosition(Dimension d);

private:
	Panel			&_panel;
	const Dimension	_geometry;

	Glyph			*_stack;
	FrameBuffer		*_current_framebuffer;
	PerfInterval	_perf;

	bool		_clip(Position p) { return ((p.x < _geometry.w) && (p.y < _geometry.h)); }
};

