/*
 * Scene management.
 */

#include <string.h>
#include "graphics.h"
#include "scene.h"
#include "panel.h"
#include "encoder.h"

Scene::Scene(Panel &p) :
	_panel(p),
	_geometry(p.dimension()),
	_stack(nullptr),
	_perf("scene draw")
{
}

void
Scene::addGlyph(Glyph *g)
{
	g->_next = _stack;
	_stack = g;
}

void
Scene::render()
{
	_perf.start();

	for (Glyph *g = _stack; g != nullptr; g = g->_next)
		g->draw();

	_perf.stop();
}

bool
Scene::event(Encoder::Event evt)
{
	return evt == Encoder::EVT_BUTTON;
}

void
Scene::fill(Region r, Colour colour)
{
	unsigned tc = r.p.x + r.d.w;
	unsigned tr = r.p.y + r.d.h;

	if (tc > _geometry.w)
		tc = _geometry.w;
	if (tr > _geometry.h)
		tr = _geometry.h;

	for (unsigned row = r.p.y; row < tr; row++)
		for (unsigned col = r.p.x; col < tc; col++)
			draw(Position(col, row), colour);
}

Position
Scene::centeredPosition(Dimension d)
{
	Position p;

	if (d.w > _geometry.w)
		d.w = _geometry.w;
	if (d.h > _geometry.h)
		d.h = _geometry.h;

	p.x = _geometry.w / 2 - d.w / 2;
	p.y = _geometry.h / 2 - d.h / 2;

	return p;
}
