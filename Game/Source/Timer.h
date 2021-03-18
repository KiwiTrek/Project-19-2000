#ifndef __TIMER_H__
#define __TIMER_H__

#include "Defs.h"

// Fast timer with milisecons precision
class Timer
{
public:
	// Constructor
	Timer();

	void Start();
	uint32 Read() const;
	float ReadSec() const;

private:
	uint32 startTime;
};

#endif //__TIMER_H__
