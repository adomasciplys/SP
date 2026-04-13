#include "collatz.hpp"

#include <doctest/doctest.h>

#include <ranges>
#include <vector>

#include <cstddef>

TEST_SUITE_BEGIN("std::generator");

TEST_CASE("Collatz Numbers")
{
    const auto vi = std::vector(std::from_range, collatz<std::size_t>(7) | std::views::take(17));
    REQUIRE(vi.size() == 17);
    CHECK(vi == std::vector<size_t>{7, 22, 11, 34, 17, 52, 26, 13, 40, 20, 10, 5, 16, 8, 4, 2, 1});
}

TEST_CASE("Collatz Numbers Empty")
{
    const auto vi = std::vector(std::from_range, collatz<std::size_t>(0) | std::views::take(1));
    REQUIRE(vi.size() == 0);
    CHECK(vi == std::vector<size_t>{});
}

TEST_CASE("Collatz Estimate")
{
    const auto res = estimate_length(3);
    CHECK(res == 8);
}

TEST_CASE("Collatz Length")
{
    const auto vi = std::vector(std::from_range, collatz_length<std::size_t>(10) | std::views::take(10));
    REQUIRE(vi.size() == 10);
    CHECK(vi == std::vector<size_t>{0, 1, 2, 8, 3, 6, 9, 17, 4, 20});
}

TEST_SUITE_END();