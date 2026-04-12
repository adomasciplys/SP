#include "generator.hpp"

#include <doctest/doctest.h>

#include <iterator>
#include <ranges>
#include <vector>
#include <concepts>  // equality_comparable

#include <cstddef>

TEST_SUITE_BEGIN("Generator");

using GenInt = Generator<int>;
using Iter = GenInt::Iterator;

// Requirements on types that can be incremented with ++
static_assert(std::weakly_incrementable<Iter>);
// // The concept std::equality_comparable specifies that the comparison operators == and != on T reflects equality: == yields true if and only if the operands are equal.
static_assert(std::equality_comparable<Iter>);
// Single pass iterator where you can read, move forward, and compare elements
static_assert(std::input_iterator<Iter>);
// Is a concept that defines the minimum requirements for a "range"
// (anything you can iterate over, like a container or a view).
static_assert(std::ranges::input_range<GenInt>);
// A forward_range is like reading a book.
// You can only flip the pages forward, but you can place a bookmark, keep reading, and then jump back to your bookmark to read that exact same section again
static_assert(std::ranges::forward_range<GenInt>);
// The concept movable<T> specifies that T is an object type that can be moved
static_assert(std::movable<GenInt>);  // important for composition with views

TEST_CASE("Fibonacci numbers")
{
    auto vi = std::vector<std::size_t>{};
    for (auto&& n : Fibonacci() | std::views::take(10))
        vi.push_back(n);
    REQUIRE(vi.size() == 10);
    CHECK(vi == std::vector<size_t>{0, 1, 1, 2, 3, 5, 8, 13, 21, 34});
}

TEST_CASE("Lucas numbers")
{
    auto vi = std::vector<size_t>{};
    for (auto&& n : Fibonacci(2, 1) | std::views::take(10))
        vi.push_back(n);
    REQUIRE(vi.size() == 10);
    CHECK(vi == std::vector<size_t>{2, 1, 3, 4, 7, 11, 18, 29, 47, 76});
}

TEST_CASE("Random")
{
    auto vi = std::vector<size_t>{};
    for (auto&& n : Random(0, 9) | std::views::take(10))
        vi.push_back(n);
    REQUIRE(vi.size() == 10);
    CHECK(vi == std::vector<size_t>{3, 7, 9, 1, 7, 7, 5, 5, 1, 4});
}

TEST_SUITE_END();