#ifndef INCLUDE_FUNCTION_TRAITS_HPP
#define INCLUDE_FUNCTION_TRAITS_HPP

#include <type_traits>
#include <tuple>

/// Checks if T is an instance of Template
template <typename T, template <class...> class Template>
struct is_instance_of : std::false_type
{};  // primary declaration

template <template <class...> class Template, typename... Args>
struct is_instance_of<Template<Args...>, Template> : std::true_type
{};  // specialization

template <typename T, template <class...> class Template>
constexpr auto is_instance_of_v = is_instance_of<T, Template>::value;  // shorthand/helper

/// Extracts function argument and return types, attempts function object by default
template <typename Fn>
struct function_traits : function_traits<decltype(&std::remove_cvref_t<Fn>::operator())>
{};

/// Free functions
template <typename R, typename... Args>
struct function_traits<R(Args...)>
{
    using return_type = R;
    using arg_types = std::tuple<Args...>;
};

/// Function pointers
template <typename R, typename... Args>
struct function_traits<R (*)(Args...)> : function_traits<R(Args...)>
{};

/// Member function pointers (const)
template <typename C, typename R, typename... Args>
struct function_traits<R (C::*)(Args...) const> : function_traits<R(Args...)>
{};

/// Member function pointers (non-const)
template <typename C, typename R, typename... Args>
struct function_traits<R (C::*)(Args...)> : function_traits<R(Args...)>
{};

/// Member function pointers (volatile/refs/cvref)
template <typename C, typename R, typename... Args>
struct function_traits<R (C::*)(Args...) volatile> : function_traits<R(Args...)>
{};

template <typename C, typename R, typename... Args>
struct function_traits<R (C::*)(Args...) const volatile> : function_traits<R(Args...)>
{};

template <typename C, typename R, typename... Args>
struct function_traits<R (C::*)(Args...)&> : function_traits<R(Args...)>
{};

template <typename C, typename R, typename... Args>
struct function_traits<R (C::*)(Args...) const&> : function_traits<R(Args...)>
{};

template <typename C, typename R, typename... Args>
struct function_traits<R (C::*)(Args...) &&> : function_traits<R(Args...)>
{};

template <typename C, typename R, typename... Args>
struct function_traits<R (C::*)(Args...) const&&> : function_traits<R(Args...)>
{};

/// Computes the return type of a function(-object)
template <auto fn>
using return_t = function_traits<decltype(fn)>::return_type;

/// Computes the type of the Ith argument of a function(-object)
template <typename Fn, std::size_t I>
using arg_t = std::tuple_element_t<I, typename function_traits<Fn>::arg_types>;

/// Computes the number of arguments of a function(-object)
template <typename Fn>
constexpr auto arg_count = std::tuple_size_v<typename function_traits<Fn>::arg_types>;

/// Computes the type of the Ith argument of a function(-objecT) value
template <auto fn, std::size_t I>
using fn_arg_t = std::tuple_element_t<I, typename function_traits<decltype(fn)>::arg_types>;

/// Computes the number of arguments of a function(-objecT) value
template <auto fn>
using fn_arg_count = std::tuple_size<typename function_traits<decltype(fn)>::arg_types>;

#endif  // INCLUDE_FUNCTION_TRAITS_HPP
