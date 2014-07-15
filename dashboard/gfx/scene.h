#pragma once

#include "graphics.h"
#include "panel.h"
#include "perf.h"
#include "encoder.h"

/* a collection of glyphs that are rendered and drawn to a panel */
class Scene
{
public:
    Scene();

    /**
     * Add the glyph to the scene glyph stack; new glyphs always stack on top.
     */
    void        add_glyph(Glyph *g);

    /**
     * Render the scene.
     */
    void    render();

    /**
     * Send an event to the scene.
     *
     * Returns true if control should pass to the next scene.
     */
    virtual bool    event(Encoder::Event evt);

    /**
     * Place a pixel in the scene.
     */
    void        draw(Position p, Colour colour) const
    {
        // XXX remove clipping test for performance?
        if (_clip(p) && _current_framebuffer)
            _current_framebuffer->draw(p, colour);
    }

    /**
     * Fill a portion of the scene.
     */
    void        fill(Region r, Colour colour) const;

    /**
     * Return the draw position for a centered item of the given dimensions.
     */
    Position    centered_position(Dimension d) const;

protected:
    virtual void    _render() const;

private:
    const Dimension _geometry;
    Glyph           *_stack;
    FrameBuffer     *_current_framebuffer;

    bool            _clip(Position p) const { return ((p.x < _geometry.w) && (p.y < _geometry.h)); }

};

class PerfScene : public Scene
{
public:

    virtual bool    event(Encoder::Event evt) override;

protected:
    virtual void    _render() const override;

private:
    static uint8_t  _index;

    static PerfItem *get_perf_item();
    static void     generator(GlyphText *gt);
};
