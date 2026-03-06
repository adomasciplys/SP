#ifndef TEMPLATES_MEASURE_HPP
#define TEMPLATES_MEASURE_HPP

#include <chrono>
#include <functional>

// Template function that accepts
// - A callable (Fn)
// - Any number of arguments (... Args)
template <typename  Fn, typename ... Args>
inline auto measure(Fn fn, Args&& ... args) // Universal reference
{
	using Clock	   = std::chrono::high_resolution_clock;
	using Duration = Clock::duration;

	// Type alias for the return type of invoking Fn with Args
	using ReturnType = std::invoke_result_t<Fn, Args...>;

	struct Result
	{
		ReturnType	 ret;
		Duration duration;
	} result;

	const auto t0	= Clock::now();
	//std::invoke(fn, std::forward<Args>(args)...):
	//    - Calls the function fn with all the arguments, preserving their original types (lvalue/rvalue) through perfect forwarding

	// std::forward<Args>(args)...
	//    - The ... unpacks the argument pack.
	//    If you passed 3 arguments, this expands to forward all 3 of them in order.
	result.ret		= std::invoke(fn, std::forward<Args>(args)...);
	const auto t1	= Clock::now();
	result.duration = t1 - t0;
	return result;
}

#endif	// TEMPLATES_MEASURE_HPP