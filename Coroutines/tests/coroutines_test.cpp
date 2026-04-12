#include "coroutines.hpp"

#include <doctest/doctest.h>

#include <iterator>
#include <ranges>
#include <vector>
#include <list>
#include <numeric>  // iota
#include <concepts>

TEST_SUITE_BEGIN("Coroutines");

TEST_CASE("std::iota")
{
    auto vi = std::vector<int>{};
    for (auto&& n : std::views::iota(0, 10))
        vi.push_back(n);
    REQUIRE(vi.size() == 10);
    CHECK(vi == std::vector{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
}

using CounterInt = Counter<int>;
using Iter = CounterInt::Iterator;

static_assert(std::weakly_incrementable<Iter>);
static_assert(std::equality_comparable<Iter>);
static_assert(std::input_iterator<Iter>);
static_assert(std::ranges::input_range<CounterInt>);
static_assert(std::ranges::forward_range<CounterInt>);
static_assert(std::movable<CounterInt>);  // for composition with views

TEST_CASE("Counter")
{
    auto vi = std::vector<int>{};
    for (auto&& n : Counter{1} | std::views::take(10))
        vi.push_back(n);
    REQUIRE(vi.size() == 10);
    CHECK(vi == std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
};

TEST_CASE("Sequence")
{
    auto vi = std::vector<int>{};
    std::ranges::copy(Sequence(0, 10), std::back_inserter(vi));
    REQUIRE(vi.size() == 10);
    CHECK(vi == std::vector{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
}

TEST_CASE("Natural numbers")
{
    const auto vi = copy_to<std::vector>(Naturals<int>() | std::views::take(10));
    REQUIRE(vi.size() == 10);
    CHECK(vi == std::vector{1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
}

TEST_CASE("Fibonacci numbers")
{
    const auto li = copy_to<std::list>(Fibonacci() | std::views::take(10));
    REQUIRE(li.size() == 10);
    CHECK(li == std::list<size_t>{0, 1, 1, 2, 3, 5, 8, 13, 21, 34});
}

TEST_SUITE_END();