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

#include <stdarg.h>
#include <limits.h>

#include "perf.h"


PerfItem *PerfItem::_list = nullptr;

PerfItem::PerfItem(const char *name) :
    _name(name),
    _next(_list)
{
    _list = reinterpret_cast<PerfItem *>(this);
}

PerfCounter::PerfCounter(const char *name) :
    PerfItem(name),
    _count(0)
{
}

void
PerfCounter::report(GlyphText *gt)
{
    gt->emitf("COUNT %s\n%u", _name, _count);
}

PerfInterval::PerfInterval(const char *name) :
    PerfCounter(name),
    _max_period(0),
    _min_period(UINT_MAX)
{
}

void
PerfInterval::start()
{
    _started = Timer::time();
}

void
PerfInterval::stop()
{
    Timer::Interval elapsed = Timer::time_since(_started);

    if (elapsed < _min_period) {
        _min_period = elapsed;
    }

    if (elapsed > _max_period) {
        _max_period = elapsed;
    }

    count();
}

void
PerfInterval::report(GlyphText *gt)
{
    gt->emitf("INTVL %s\n%u\n %u\n %u", _name, _count, _min_period, _max_period);
}

extern unsigned _bss_end;

void
PerfMem::report(GlyphText *gt)
{
    unsigned *fp = &_bss_end;
    unsigned count = 0;

    while (*fp++ == 0xffffffff) {
        count += 4;
    }

    gt->emitf("FREE %C%u", Blue, count);
}
