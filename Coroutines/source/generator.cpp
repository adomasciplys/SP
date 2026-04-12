#include "generator.hpp"

#include <concepts>
#include <utility>  // exchange
#include <random>

/// Uses coroutines to implement the infinite Fibonacci sequence
template <std::integral Number>
Generator<Number> Fibonacci(Number n0, Number n1)
{
    for (;;) {
        co_yield n0;
        n0 = std::exchange(n1, n0 + n1);
    }
}

/// Explicit template instantiation for common types
template Generator<size_t> Fibonacci<size_t>(size_t, size_t);
template Generator<int> Fibonacci<int>(int, int);

template <std::integral Number>
Generator<Number> Random(Number from, Number to)
{
    auto gen = std::mt19937{42};
    auto dist = std::uniform_int_distribution<Number>{from, to};
    for (;;)
        co_yield dist(gen);
}

template Generator<std::size_t> Random<std::size_t>(std::size_t from, std::size_t to);
template Generator<int> Random<int>(int from, int to);
