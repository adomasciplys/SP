#ifndef COROUTINES_CORO_HPP
#define COROUTINES_CORO_HPP

#include "coro/generator.hpp"

#include <concepts>

#include <cstddef>  // std::size_t
#include <limits>

/// Integer sequence as std::ranges::input_range, just like std::views::iota
template <std::integral Number = std::size_t>
struct Counter
{
    struct Sentinel;
    struct Iterator
    {
        /// weakly_incrementable:
        using difference_type = std::ptrdiff_t;
        Iterator& operator++()  ///< pre-increment
        {
            if (_state < _bound)
                ++_state;
            return *this;
        }
        Iterator operator++(int) { return operator++(); }  ///< post-increment
        /// equality_comparable:
        bool operator==(const Iterator& other) const noexcept = default;
        bool operator==(const Sentinel&) const noexcept { return _state == _bound; }
        /// input_range:
        using value_type = Number;
        const Number& operator*() const { return _state; }
        Number _state{};
        Number _bound{};
    };
    struct Sentinel
    {
        bool operator==(const Iterator& it) const { return it._counter->state == it._counter->bound; }
    };
    Iterator begin() const { return Iterator{_initial, _bound}; }
    static Sentinel end() { return {}; }
    explicit Counter(Number first = 0, Number bound = std::numeric_limits<Number>::max()):
        _initial{first}, _bound{bound}
    {}

private:
    Number _initial{};
    Number _bound{};
};

/// Class Template Argument Deduction (CTAD) guide:
template <std::integral Number>
Counter(Number) -> Counter<Number>;

template <std::integral Number>
Counter(Number, Number) -> Counter<Number>;

template <std::integral Number = std::size_t>
coro::generator<Number> Sequence(Number from = 0, Number to = std::numeric_limits<Number>::max());

template <std::integral Number = std::size_t>
coro::generator<Number> Naturals();

template <std::integral Number = std::size_t>
coro::generator<Number> Fibonacci(Number n0 = 0, Number n1 = 1);

/// Helper similar to std::ranges::to<template> (C++23), but using only C++20
template <template <typename...> class Result, std::ranges::input_range Input>
auto copy_to(Input&& input)
{
    auto res = Result<std::ranges::range_value_t<Input>>();
    std::ranges::copy(std::forward<Input>(input), std::back_inserter(res));
    return res;
}

#endif  // COROUTINES_CORO_HPP