#include "PerfTimer.h"

#include "SDL_timer.h"

uint64 PerfTimer::frequency = 0;

PerfTimer::PerfTimer()
{
	if (frequency == 0)
	{
		frequency = SDL_GetPerformanceFrequency();
	}

	Start();
}

void PerfTimer::Start()
{
	startTick = SDL_GetPerformanceCounter();
}

double PerfTimer::ReadMs() const
{
	return (1000.0 * (double(SDL_GetPerformanceCounter() - startTick) / double(frequency)));
}

uint64 PerfTimer::ReadTicks() const
{
	return (SDL_GetPerformanceCounter() - startTick);
}