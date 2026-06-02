#include "fmap_optional.hpp"
#include "functions.hpp"

#include <doctest/doctest.h>

#include <vector>

/// Test for is_optional
static_assert(is_optional<std::optional<int>>);  // positive test
static_assert(!is_optional<int>);                // negative test
static_assert(!is_optional<std::vector<int>>);   // negative test

TEST_SUITE_BEGIN("fmap_optional");

TEST_CASE("square<double>")
{
    auto square_double_opt = fmap_optional(square<double>);
    const auto res = square_double_opt(5);
    static_assert(std::same_as<decltype(res), const std::optional<double>>);
    REQUIRE(res);
    CHECK(*res == 25);
}

TEST_CASE("square_int")
{
    auto square_int_opt = fmap_optional(square_int);
    const auto res = square_int_opt(5);
    static_assert(std::same_as<decltype(res), const std::optional<int>>);
    REQUIRE(res);
    CHECK(*res == 25);
}

TEST_CASE("square_lambda")
{
    auto square_lambda_opt = fmap_optional(square_lambda);
    const auto res = square_lambda_opt(5);
    static_assert(std::same_as<decltype(res), const std::optional<double>>);
    REQUIRE(res);
    CHECK(*res == 25);
}

TEST_SUITE_END();
