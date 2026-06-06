#ifndef TEMPLATES_MEASURE_HPP
#define TEMPLATES_MEASURE_HPP

#include <chrono>

/// Measures the time it takes to call fn with arg. Returns the result with duration.
/// TODO: Convert this function into a function template, where the fn, arg(s) and return types are
/// deduced
/// TODO: Replace the type of fn with deduced type
/// TODO: Replace the type of arg with deduced type
/// TODO EXTRA: Extend the function template to arbitrary number of arguments
inline auto measure(MinMax (&fn)(std::span<const double>), std::span<const double> arg)
{
	// TODO: use std::invoke_result_t to extract the return type of fn (assume non-void type)
	using Clock	   = std::chrono::high_resolution_clock;
	using Duration = Clock::duration;
	struct Result
	{
		MinMax	 ret;  /// TODO: replace MinMax with deduced type
		Duration duration;
	} result;
	const auto t0	= Clock::now();
	result.ret		= fn(arg);	// TODO: use std::forward to forward the arguments to fn call
	const auto t1	= Clock::now();
	result.duration = t1 - t0;
	return result;
}

#endif	// TEMPLATES_MEASURE_HPP