#pragma once

#include "timer.h"

class PerfItem
{
public:
	enum Kind : uint8_t {
		COUNT,
		INTERVAL,
		LOAD
	};

	PerfItem(const char *name, Kind kind);

	static void	print_report();

protected:
	virtual void	report() = 0;

private:
	static PerfItem	*_list;
	const char * const _name;
	const Kind _kind;

	PerfItem	* const _next;
};

class PerfCounter : public PerfItem
{
public:
	PerfCounter(const char *name, Kind = COUNT);

	unsigned operator++() { return ++_count; }
	unsigned operator++(int junk) { return _count++; }
	void		count() { _count++; }

protected:
	virtual void	report() override;
	volatile unsigned _count;
};

class PerfInterval : public PerfCounter
{
public:
	PerfInterval(const char *name);

	void		start();
	void		stop();

protected:
	virtual void	report() override;

private:
	Timer::Interval _max_period;
	Timer::Interval _min_period;
	Timer::Time	_started;
};

class PerfLoad : public PerfItem
{
public:
	PerfLoad(const char *name);

	void		start();
	void		stop();

protected:
	virtual void	report() override;

private:
	Timer::Time 	_active;
	Timer::Time 	_inactive;
	Timer::Time	_changed;
};
