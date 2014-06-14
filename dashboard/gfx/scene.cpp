/*
 * Scene management.
 */

#include <string.h>
#include "graphics.h"
#include "scene.h"
#include "panel.h"
#include "encoder.h"
#include "fonts.h"

extern Panel gPanel;

Scene::Scene(const char *name) :
    _geometry(gPanel.dimension()),
    _stack(nullptr),
    _current_framebuffer(nullptr),
    _perf(name)
{
}

void
Scene::addGlyph(Glyph *g)
{
    g->push(_stack);
    _stack = g;
}

void
Scene::render()
{
    // cache the current draw buffer; bail out if we didn't get one
    if ((_current_framebuffer = gPanel.get_draw_buffer()) == nullptr) {
        return;
    }

    _perf.start();

    _current_framebuffer->clear();
    _render();
    gPanel.push_draw_buffer();

    _perf.stop();
}

void
Scene::_render()
{
    for (Glyph *g = _stack; g != nullptr; g = g->next())
        g->draw(this);
}

bool
Scene::event(Encoder::Event evt)
{
    switch (evt) {
    case Encoder::EVT_BUTTON:
        return true;
    case Encoder::EVT_DOWN:
        gPanel.dimmer();
        break;
    case Encoder::EVT_UP:
        gPanel.brighter();
        break;
    default:
        break;
    }
    return false;}

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

uint8_t PerfScene::_index;

void
PerfScene::_render()
{
    GlyphText t(nullptr, Region(0, 0, 64, 32), font_Misc_Fixed_Medium_4x6, DimGreen, PerfScene::generator);
    t.draw(this);
}

void
PerfScene::generator(GlyphText *gt)
{
    PerfItem *pi = get_perf_item();

    pi->report(gt);
}

bool
PerfScene::event(Encoder::Event evt)
{
    switch (evt) {
    case Encoder::EVT_BUTTON:
        return true;
    case Encoder::EVT_DOWN:
        if (_index > 0) {
            _index--;
        }
        break;
    case Encoder::EVT_UP:
        _index++;
        if (get_perf_item() == nullptr) {
            _index--;
        }
        break;
    default:
        break;
    }
    return false;
}

PerfItem *
PerfScene::get_perf_item()
{
    PerfItem *pi = PerfItem::first();

    for (unsigned i = 0; i < _index; i++) {
        if (pi == nullptr)
            break;
        pi = pi->next();
    }
    return pi;
}

PerfScene   gPerf;
