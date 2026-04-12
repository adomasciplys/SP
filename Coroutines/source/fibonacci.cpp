#include "fibonacci.hpp"

#include <concepts>
#include <generator>  // C++23

/// Uses coroutines to implement an infinite range of Fibonacci numbers
template <std::integral Number>
std::generator<Number> fibonacci(Number n0, Number n1)
{
    for (;;) {
        co_yield n0;
        n0 = std::exchange(n1, n0 + n1);
    }
}

/// Explicit template instantiations to cover common types
template std::generator<size_t> fibonacci<size_t>(size_t, size_t);
template std::generator<int> fibonacci<int>(int, int);
