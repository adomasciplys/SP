#ifndef COROUTINES_FIBONACCI_HPP
#define COROUTINES_FIBONACCI_HPP

#include <generator>  // C++23

#include <cstddef>

/// Demonstrates the use of std::generator, @returns an infinite range of Fibonacci numbers
template <std::integral Number = std::size_t>
std::generator<Number> fibonacci(Number n0 = 0, Number n1 = 1);

#endif  // COROUTINES_FIBONACCI_HPP
