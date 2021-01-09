#pragma once

#include <chrono>
#include <ctime>

template <typename T>
class basic_stopwatch
{
	typedef T clock;
	typename clock::time_point p;
	typename clock::duration   d;

public:
	void tick() { p = clock::now(); }
	void tock() { d += clock::now() - p; }
	void reset() { d = clock::duration::zero(); }

	template <typename S> 
	unsigned long long int report() const
	{
		return std::chrono::duration_cast<S>(d).count();
	}

	unsigned long long int report_ms() const
	{
		return report<std::chrono::milliseconds>();
	}

	basic_stopwatch() : p(), d() { }
};

struct c_clock
{
	typedef std::clock_t time_point;
	typedef std::clock_t duration;
	static time_point now()
	{
		return std::clock();
	}
};

template <>
unsigned long long int basic_stopwatch<c_clock>::report_ms() const
{
	return 1000. * double(d) / double(CLOCKS_PER_SEC);
}

typedef basic_stopwatch<std::chrono::high_resolution_clock> stopwatch;
typedef basic_stopwatch<c_clock> cstopwatch;