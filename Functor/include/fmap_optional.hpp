#ifndef INCLUDE_FMAP_OPTIONAL_HPP
#define INCLUDE_FMAP_OPTIONAL_HPP

#include "function_traits.hpp"

#include <optional>

/// Checks if T is an instance of std::optional
template <typename T>
concept is_optional = is_instance_of_v<std::remove_cvref_t<T>, std::optional>;

/// Lifts a free function fn to std::optional types
template <typename Arg, typename FnRet>
auto fmap_optional(FnRet (&fn)(Arg))
{
    static_assert(!std::is_reference_v<Arg>, "Expecting a function with a non-reference argument");
    static_assert(!std::is_pointer_v<Arg>, "Expecting a function a non-pointer argument");
    static_assert(!std::is_reference_v<FnRet>, "Expecting a function with a non-reference return type");
    static_assert(!std::is_pointer_v<FnRet>, "Expecting a function with a non-pointer return type");
    using Ret = std::conditional_t<is_optional<FnRet>, FnRet, std::optional<FnRet>>;
    return [&fn](std::optional<Arg> arg) { return arg ? Ret{fn(*arg)} : Ret{}; };
}

/// Lifts a function object fn for std::optional types
template <typename Fn>
auto fmap_optional(Fn&& fn)
{
    using FnTraits = function_traits<Fn>;
    using FnArgs = FnTraits::arg_types;
    static_assert(std::tuple_size_v<FnArgs> == 1, "Expecting a function with a single argument");
    using Arg = std::tuple_element_t<0, FnArgs>;
    static_assert(!std::is_reference_v<Arg>, "Expecting a function with a non-reference argument");
    static_assert(!std::is_pointer_v<Arg>, "Expecting a function with a non-pointer argument");
    using FnRet = FnTraits::return_type;
    static_assert(!std::is_reference_v<FnRet>, "Expecting a function with a non-reference return type");
    static_assert(!std::is_pointer_v<FnRet>, "Expecting a function with a non-pointer return type");
    using Ret = std::conditional_t<is_optional<FnRet>, FnRet, std::optional<FnRet>>;
    return [fn = std::forward<Fn>(fn)](std::optional<Arg> arg) { return arg ? Ret{fn(*arg)} : Ret{}; };
}

#endif  // INCLUDE_FMAP_OPTIONAL_HPP
