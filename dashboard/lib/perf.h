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

class PerfLoad : public PerfItem
{
public:
    PerfLoad(const char *name);

    void            start();
    void            stop();

protected:
    virtual void    report(GlyphText *gt) override;

private:
    Timer::Interval _active;
    Timer::Interval _inactive;
    Timer::Time     _changed;
};

class PerfMem : public PerfItem
{
public:
    PerfMem() : PerfItem("MEM") {}

    virtual void    report(GlyphText *gt) override;
};
