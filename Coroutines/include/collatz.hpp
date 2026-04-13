#ifndef COROUTINES_COLLATZ_HPP
#define COROUTINES_COLLATZ_HPP

#include <generator>  // C++23

/// Demonstrates the use of std::generator, @returns an infinite range of Collatz numbers
template <std::integral Number = std::size_t>
std::generator<Number> collatz(Number start = 1);


#endif  // COROUTINES_COLLATZ_HPP
