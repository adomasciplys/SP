#include "function_traits.hpp"
#include "functions.hpp"

#include <doctest/doctest.h>

#include <optional>
#include <vector>

/// Test is_instance_of
static_assert(is_instance_of_v<std::optional<int>, std::optional>);  // positive test
static_assert(!is_instance_of_v<std::vector<int>, std::optional>);   // negative test

static_assert(!is_instance_of_v<std::optional<int>, std::vector>);  // negative test
static_assert(is_instance_of_v<std::vector<int>, std::vector>);     // positive test

/// Test fn_arg_t
static_assert(std::same_as<fn_arg_t<square_int, 0>, int>);
static_assert(std::same_as<fn_arg_t<square<double>, 0>, double>);
static_assert(std::same_as<fn_arg_t<square_lambda, 0>, double>);

/// Test return_t
static_assert(std::same_as<return_t<square_int>, int>);
static_assert(std::same_as<return_t<square<double>>, double>);
static_assert(std::same_as<return_t<square_lambda>, double>);

/// Alternative (simpler) way of extracting the return type of the function:
static_assert(std::same_as<std::invoke_result_t<decltype(square_int), int>, int>);
static_assert(std::same_as<std::invoke_result_t<decltype(square<double>), double>, double>);
static_assert(std::same_as<std::invoke_result_t<decltype(square_lambda), double>, double>);

TEST_CASE("function_traits") {}