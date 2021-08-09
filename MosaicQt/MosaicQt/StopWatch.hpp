#pragma once

#include <chrono>
#include <ctime>

template <typename T>
class  StopWatch
{
	using clock = T;
	typename clock::time_point timePoint;
	typename clock::duration   duration;

public:
	void tick() { timePoint = clock::now(); }
	void tock() { duration += clock::now() - timePoint; }
	void reset() { duration = clock::duration::zero(); }

	template <typename S>
	[[nodiscard]] auto report() const
	{
		return std::chrono::duration_cast<S>(duration).count();
	}

	[[nodiscard]] auto report_ms() const
	{
		return report<std::chrono::milliseconds>();
	}

	StopWatch() : timePoint(), duration() { }
};

struct Clock
{
	using time_point = std::clock_t;

	using duration = std::clock_t;
	static time_point now() {
		return std::clock();
	}
};

template <>
inline auto StopWatch<Clock>::report_ms() const
{
	return 1000. * static_cast<double>(duration) / static_cast<double>(CLOCKS_PER_SEC);
}

using stopwatch = StopWatch<std::chrono::high_resolution_clock>;
using  cstopwatch = StopWatch<Clock>;

