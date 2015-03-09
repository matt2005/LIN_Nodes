/*
 * Copyright (c) 2012-2015, Mike Smith, <msmith@purgatory.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * o Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
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

Scene::Scene() :
    _geometry(gPanel.dimension()),
    _stack(nullptr),
    _current_framebuffer(nullptr)
{
}

void
Scene::add_glyph(Glyph *g)
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

    _current_framebuffer->clear();
    _render();
    gPanel.push_draw_buffer();
}

void
Scene::_render() const
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

    return false;
}

void
Scene::fill(Region r, Colour colour) const
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
Scene::centered_position(Dimension d) const
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
PerfScene::_render() const
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
