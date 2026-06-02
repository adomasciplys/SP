#ifndef INCLUDE_FMAP_VECTOR_HPP
#define INCLUDE_FMAP_VECTOR_HPP

#include "function_traits.hpp"

#include <vector>
#include <ranges>

/// Checks if T is an instance of std::vector
template <typename T>
concept is_vector = is_instance_of_v<std::remove_cvref_t<T>, std::vector>;


/// Lifts a free function fn to std::vector types
// 'Arg' represents the input data type that the free function accepts.
// 'FnRet' represents the return data type that the free function outputs.
//  E.g. int double_value(int x) { return x * 2; } is a free function
template <typename Arg, typename FnRet>
auto fmap_vector(FnRet (&fn)(Arg))
{
    // Returns a lambda expression that captures the free function 'fn' by reference.
    // This lambda expression accepts an input vector named 'args'.
    return [&fn](const std::vector<Arg>& args) {

        // Processes the data using a passive pipeline:
        // 1. 'args' is the input vector.
        // 2. 'std::views::transform(fn)' creates a view that applies 'fn' to elements on the fly.
        // 3. 'std::ranges::to' executes the view and saves the new values into a new vector.
        return args | std::views::transform(fn) | std::ranges::to<std::vector<FnRet>>();
    };
}

/// Lifts a function object fn for std::vector types
template <typename Fn>
auto fmap_vector(Fn&& fn)
{
    // 1. Use function_traits to inspect the data types of the passed callable object.
    using FnTraits = function_traits<Fn>;

    // 2. Extract the list of input argument types as a std::tuple.
    using FnArgs = FnTraits::arg_types;

    // 3. Ensure the callable object accepts exactly one argument at compile time.
    static_assert(std::tuple_size_v<FnArgs> == 1, "Expecting a function with a single argument");

    // 4. Extract the specific data type of the first argument (index 0).
    using Arg = std::tuple_element_t<0, FnArgs>;

    // 5. Extract the specific return data type of the callable object.
    using FnRet = FnTraits::return_type;

    // 6. Return a lambda expression.
    // 'std::forward' safely transfers the callable object 'fn' into the capture.
    return [fn = std::forward<Fn>(fn)](const std::vector<Arg>& args)
    {
        // 7. Run the processing pipeline:
        // Read from 'args', apply 'fn' on the fly, and save results into a new vector.
        return args | std::views::transform(fn) | std::ranges::to<std::vector<FnRet>>();
    };
}

#endif  // INCLUDE_FMAP_VECTOR_HPP
