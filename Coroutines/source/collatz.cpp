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

auto estimate_length(std::size_t n) -> std::size_t {
    std::size_t len = 0;
    for (auto val : collatz(n)) {
        ++len;
        if (val == 1) break;
    }
    return len;
}

template <std::integral Number>
std::generator<Number> collatz_length(Number number_count)
{
    auto counter = 0;

    if (number_count == counter) {
        co_return;
    }

    for (;;) {
        co_yield estimate_length(counter);
        counter += 1;
    }
}

template <std::integral Number>
std::generator<Number> max_collatz()
{
    Number current_max = 0;
    for (Number i = 0; ; ++i) {
        Number len = estimate_length(i);
        if (len > current_max) {
            current_max = len;
        }
        co_yield current_max;
    }
}

/// Explicit template instantiations to cover common types
template std::generator<size_t> collatz<size_t>(size_t);
template std::generator<int> collatz<int>(int);
template std::generator<size_t> collatz_length<size_t>(size_t);
template std::generator<size_t> max_collatz<size_t>();
