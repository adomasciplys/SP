#include "minmax.hpp"
#include "random.hpp"

#include <doctest/doctest.h>

#include <algorithm>
#include <iomanip>

/// Deterministic input for tests:
static const auto test_input = make_random(5);

/// Test for if_cond function (example use-case)
static void test_if_cond()
{
	const auto res = if_cond(test_input);
	// std::cout << std::setprecision(17) << res << std::endl;
	CHECK(res.min == -727.45463273512576);
	CHECK(res.max == 806.53793285675647);
}

/// Test for minmax function (example use-case)
static void test_minmax()
{
	const auto res = minmax(test_input);
	CHECK(res.min == -727.45463273512576);
	CHECK(res.max == 806.53793285675647);
}

/// Test for element function (example use-case)
static void test_element()
{
	const auto res = element(test_input);
	CHECK(res.min == -727.45463273512576);
	CHECK(res.max == 806.53793285675647);
}

/// Test for element function (example use-case)
static void test_ranges()
{
	const auto res = ranges(test_input);
	CHECK(res.min == -727.45463273512576);
	CHECK(res.max == 806.53793285675647);
}

TEST_SUITE_BEGIN("Finding min and max");

TEST_CASE("if_cond") { test_if_cond(); }

TEST_CASE("minmax") { test_minmax(); }

TEST_CASE("element") { test_element(); }

TEST_CASE("ranges") { test_ranges(); }

/* TODO: refactor test_* functions into one function template, so that the following code works:
TEST_CASE("if_cond")
{
	test(if_cond);
}

TEST_CASE("minmax")
{
	test(minmax);
}

TEST_CASE("element")
{
	test(element);
}

TEST_CASE("ranges")
{
	test(ranges);
}
*/

TEST_SUITE_END();