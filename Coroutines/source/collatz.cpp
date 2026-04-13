#include "collatz.hpp"

#include <concepts>
#include <generator>  // C++23

/// Uses coroutines to implement an infinite range of Collatz numbers
template <std::integral Number>
std::generator<Number> collatz(Number start)
{
    if (start == 0) {
        co_return;
    }
    for (;;) {
        co_yield start;
        if ( start % 2 == 0 ) {
            start /= 2;
        }else {
            start = (start * 3) + 1;
        }
    }
}

/// Explicit template instantiations to cover common types
template std::generator<size_t> collatz<size_t>(size_t);
template std::generator<int> collatz<int>(int);
