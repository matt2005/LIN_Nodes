#include <stdarg.h>
#include <limits.h>

#include "perf.h"
#include "debug.h"


PerfItem *PerfItem::_list = nullptr;

PerfItem::PerfItem(const char *name, Kind kind) :
    _name(name),
    _kind(kind),
    _next(_list)
{
    _list = (PerfItem *)this;
}

void
PerfItem::print_report()
{
    debug("\n### perf ###");

    PerfItem *p = _list;

    while (p != nullptr) {
        debug_nonl("%s:%s", p->_name, &"                "[(strlen(p->_name) < 16) ? strlen(p->_name) : 16]);
        p->report();
        p = p->_next;
    }
}

PerfCounter::PerfCounter(const char *name, Kind kind) :
    PerfItem(name, kind),
    _count(0)
{
}

void
PerfCounter::report()
{
    debug("%u", _count);
}

PerfInterval::PerfInterval(const char *name) :
    PerfCounter(name, INTERVAL),
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
    Timer::Interval elapsed = Timer::timeSince(_started);

    if (elapsed < _min_period) {
        _min_period = elapsed;
    }

    if (elapsed > _max_period) {
        _max_period = elapsed;
    }

    count();
}

void
PerfInterval::report()
{
    debug("%u: %u-%uus", _count_min_period, _max_period);
}

PerfLoad::PerfLoad(const char *name) :
    PerfItem(name, LOAD),
    _active(0),
    _inactive(0),
    _changed(0)
{
}

void
PerfLoad::start()
{
    if (_changed > 0) {
        _inactive += Timer::timeSince(_changed);
    }

    _changed = Timer::time();
}

void
PerfLoad::stop()
{
    if (_changed > 0) {
        _active += Timer::timeSince(_changed);
    }

    _changed = Timer::time();
}

void
PerfLoad::report()
{
    debug("%llu%%", (unsigned)((_active * 100) / (_active + _inactive)));
}
