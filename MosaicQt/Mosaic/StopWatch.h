#pragma once

#include <chrono>
#include <ctime>


#ifdef STOPWATCH_EXPORTS
#define STOPWATCH_API __declspec(dllexport)
#else
#define STOPWATCH_API __declspec(dllimport)
#endif

template <typename T>
class STOPWATCH_API basic_stopwatch
{
	using clock = T;
	typename clock::time_point p;
	typename clock::duration   d;

public:
	void tick() { p = clock::now(); }
	void tock() { d += clock::now() - p; }
	void reset() { d = clock::duration::zero(); }

	template <typename S>
	auto report() const
	{
		return std::chrono::duration_cast<S>(d).count();
	}

	auto report_ms() const
	{
		return report<std::chrono::milliseconds>();
	}

	basic_stopwatch() : p(), d() { }
};

struct c_clock
{
	using time_point = std::clock_t;

	using duration = std::clock_t;
	static time_point now()
	{
		return std::clock();
	}
};

template <>
auto basic_stopwatch<c_clock>::report_ms() const
{
	return 1000. * double(d) / double(CLOCKS_PER_SEC);
}

using stopwatch = basic_stopwatch<std::chrono::high_resolution_clock>;
using  cstopwatch = basic_stopwatch<c_clock>;