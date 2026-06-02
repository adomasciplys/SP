#include "functions.hpp"
#include "fmap_optional.hpp"
#include "fmap_vector.hpp"

#include <doctest/doctest.h>

TEST_SUITE_BEGIN("fmap_vector");

/// Test for is_vector
static_assert(is_vector<std::vector<int>>);  // positive test
static_assert(!is_vector<int>); // negative test
static_assert(!is_vector<std::optional<int>>); // negative test


TEST_CASE("square<double>")
{
    auto square_vector = fmap_vector(square<double>);
    const auto input = std::vector{1., 2., 3.};
    const auto res = square_vector(input);
    static_assert(is_vector<decltype(res)>);
    REQUIRE(res.size() == input.size());
    CHECK(res[0] == 1);
    CHECK(res[1] == 4);
    CHECK(res[2] == 9);
}

TEST_CASE("square_int")
{
    auto square_vector = fmap_vector(square_int);
    const auto input = std::vector{1,2,3};
    const auto res = square_vector(input);
    static_assert(is_vector<decltype(res)>);
    REQUIRE(res.size() == input.size());
    CHECK(res[0] == 1);
    CHECK(res[1] == 4);
    CHECK(res[2] == 9);
}

TEST_CASE("square_lambda")
{
    auto square_vector = fmap_vector(square_lambda);
    const auto input = std::vector{1., 2., 3.};
    const auto res = square_vector(input);
    static_assert(is_vector<decltype(res)>);
    REQUIRE(res.size() == input.size());
    CHECK(res[0] == 1);
    CHECK(res[1] == 4);
    CHECK(res[2] == 9);
}

TEST_CASE("fmap_optional(square_root<double>)")
{
    auto sqrt_vector = fmap_vector(fmap_optional(square_root<double>));
    const auto input = std::vector<std::optional<double>>{4.,9.,-1.};
    const auto res = sqrt_vector(input);
    static_assert(is_vector<decltype(res)>);
    REQUIRE(res.size() == input.size());
    static_assert(is_optional<decltype(res[0])>);
    REQUIRE(res[0].has_value());
    CHECK(*res[0] == 2);
    REQUIRE(res[1].has_value());
    CHECK(*res[1] == 3);
    REQUIRE(!res[2].has_value());
}

TEST_CASE("fmap_optional(square_root<int>)")
{
    auto sqrt_vector = fmap_vector(fmap_optional(square_root<int>));
    const auto input = std::vector<std::optional<int>>{4,9,-1.};
    const auto res = sqrt_vector(input);
    static_assert(is_vector<decltype(res)>);
    REQUIRE(res.size() == input.size());
    static_assert(is_optional<decltype(res[0])>);
    REQUIRE(res[0].has_value());
    CHECK(*res[0] == 2);
    REQUIRE(res[1].has_value());
    CHECK(*res[1] == 3);
    REQUIRE(!res[2].has_value());
}

TEST_SUITE_END();