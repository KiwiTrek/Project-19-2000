#ifndef __PERFTIMER_H__
#define __PERFTIMER_H__

#include "Defs.h"

// Slow timer with microsecond precision
class PerfTimer
{
public:
	// Constructor
	PerfTimer();

	void Start();
	double ReadMs() const;
	uint64 ReadTicks() const;

private:
	uint64 startTick;
	static uint64 frequency;
};

#endif // __PERFTIMER_H__
