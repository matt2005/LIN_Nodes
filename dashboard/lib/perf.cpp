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
