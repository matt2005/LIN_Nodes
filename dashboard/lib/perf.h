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

#pragma once

#include "timer.h"
#include "graphics.h"

class PerfItem
{
public:

    PerfItem(const char *name);

    static PerfItem *first() { return _list; }
    PerfItem        *next() { return _next; }

    virtual void    report(GlyphText *gt) = 0;

protected:
    const char *const _name;

private:
    static PerfItem *_list;

    PerfItem     *const _next;
};

class PerfCounter : public PerfItem
{
public:
    PerfCounter(const char *name);

    unsigned operator++() { return ++_count; }
    unsigned operator++(int junk) { return _count++; }
    void            count() { _count++; }

protected:
    virtual void    report(GlyphText *gt) override;
    volatile unsigned _count;
};

class PerfInterval : public PerfCounter
{
public:
    PerfInterval(const char *name);

    void            start();
    void            stop();

protected:
    virtual void    report(GlyphText *gt) override;

private:
    Timer::Interval _max_period;
    Timer::Interval _min_period;
    Timer::Time _started;
};

class PerfMem : public PerfItem
{
public:
    PerfMem() : PerfItem("MEM") {}

    virtual void    report(GlyphText *gt) override;
};
