#include "StopWatch.h"

Clock::time_point Clock::now()
{
	return std::clock();
}
