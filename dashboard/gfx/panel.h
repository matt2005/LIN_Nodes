/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <msmith@purgatory.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#pragma once

#include "graphics.h"
#include "perf.h"

#include "timer.h"

/* a physical display */
class Panel
{
public:
    Panel();

    static Dimension    dimension() { return Dimension(FrameBuffer::columns(), FrameBuffer::rows()); }

    /*
     * Adjust brightness down.
     */
    bool        dimmer()
    {
        if ((_max_brightness >> (_dim_level + 1)) > 0) {
            _dim_level++;
            return true;
        }

        return false;
    }

    /*
     * Adjust brightness up.
     */
    bool        brighter()
    {
        if (_dim_level > 0) {
            _dim_level--;
            return true;
        }

        return false;
    }

    /*
     * Get/set current dimming level.
     */
    uint8_t     &dimming() { return _dim_level; }

    /**
     * Get the currently-drawable framebuffer.
     *
     * Once this returns !nullptr, it will continue to return the same value
     * until push_buffer() is called.
     *
     * @return          Pointer to the currently drawable framebuffer, or nullptr
     *                  if there is no buffer available.
     */
    FrameBuffer *get_draw_buffer() { return select_buffer(_fb_available); }

    /**
     * Push the currently-drawable framebuffer to the ready state.
     *
     * Make sure that _fb_available is ready to be overwritten as soon
     * as _fb_ready is updated, since the interrupt handler may claim it
     * immediately.
     */
    void        push_draw_buffer()
    {
        if ((_fb_ready == kNoBuf) && (_fb_available != kNoBuf)) {
            BufferIndex scratch = _fb_available;
            _fb_available = kNoBuf;
            _fb_ready = scratch;
        }
    }


private:
    // Brightness is expressed in terms of the on period for the LSB.
    // Assuming a frame time of ~16ms, two rows per line and linear
    // brightness scaling, determine the LSB period for the given depth.
    // Adjust the 16ms value for the ~6ms worth of pixel transfer time...
    static const unsigned _max_brightness = ((10000 / (FrameBuffer::rows() / 2)) >> PaletteEntry::depth);

    enum BufferIndex : uint8_t {
        kBuf0 = 0,
        kBuf1 = 1,
        kNoBuf = 2
    };
    FrameBuffer     _buffer[2];

    Timer           _timer;

    union {
        unsigned        counter;
        struct {
            unsigned    is_dimming: 1;
            unsigned    row: 31 - __builtin_clz(FrameBuffer::rows() / 2);
            unsigned    slot: 32 - __builtin_clz(PaletteEntry::depth);
        };
    }               _phase;

    volatile BufferIndex _fb_available;     // available for client to draw into
    volatile BufferIndex _fb_ready;         // ready to be displayed
    volatile BufferIndex _fb_active;        // actively being displayed

    uint8_t         _dim_level;

    static void     tick(void *arg);
    void            _tick();
    Timer::Interval phase_advance();

    static void     line_init();
    static void     line_off();
    void            line_update(unsigned row, unsigned slot);

    FrameBuffer     *select_buffer(BufferIndex bufidx)
    {
        switch (bufidx) {
        case kBuf0:
            return &_buffer[0];

        case kBuf1:
            return &_buffer[1];

        default:
            return nullptr;
        }
    }
};
