#include "fibonacci.hpp"

#include <doctest/doctest.h>

#include <ranges>
#include <vector>

#include <cstddef>

TEST_SUITE_BEGIN("std::generator");

TEST_CASE("Fibonacci numbers")
{
    const auto vi = std::vector(std::from_range, fibonacci<std::size_t>() | std::views::take(10));
    REQUIRE(vi.size() == 10);
    CHECK(vi == std::vector<size_t>{0, 1, 1, 2, 3, 5, 8, 13, 21, 34});
}

TEST_CASE("Lucas numbers")
{
    const auto vi = fibonacci(2, 1) | std::views::take(10) | std::ranges::to<std::vector>();
    REQUIRE(vi.size() == 10);
    CHECK(vi == std::vector{2, 1, 3, 4, 7, 11, 18, 29, 47, 76});
}

TEST_SUITE_END();