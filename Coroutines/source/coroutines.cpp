#include "coroutines.hpp"

template <std::integral Number>
coro::generator<Number> Sequence(Number from, Number to)
{
    for (auto i = from; i < to; ++i)
        co_yield i;
    co_return;
}
/// Explicit template instantiation for common integer types
template coro::generator<size_t> Sequence<size_t>(size_t from, size_t to);
template coro::generator<int> Sequence<int>(int from, int to);

template <std::integral Number>
coro::generator<Number> Naturals()
{
    auto n = Number{1};
    for (;;)
        co_yield n++;
}
/// Explicit template instantiation for common integer types
template coro::generator<size_t> Naturals<size_t>();
template coro::generator<int> Naturals<int>();

template <std::integral Number>
coro::generator<Number> Fibonacci(Number n0, Number n1)
{
    while (true) {
        co_yield n0;
        n0 = std::exchange(n1, n0 + n1);
    }
}
/// Explicit template instantiation for common integer types
template coro::generator<size_t> Fibonacci(size_t n0, size_t n1);
template coro::generator<int> Fibonacci(int n0, int n1);
