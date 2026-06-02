/// Provides some algebraic functions with one argument
#ifndef INCLUDE_FUNCTIONS_HPP
#define INCLUDE_FUNCTIONS_HPP

#include <optional>

#include <cmath>  // sqrt

template <typename T>
    requires(std::is_arithmetic_v<T>)
T square(T x)
{
    return x * x;
}  // faster than std::pow(x,2)

inline int square_int(int x) { return x * x; }
inline auto square_lambda = [](double x) { return square(x); };

template <typename T>
    requires(std::is_arithmetic_v<T>)
std::optional<T> square_root(T x)
{
    if constexpr (std::floating_point<T> || std::signed_integral<T>) {
        if (x < 0)
            return {};
    }
    return std::sqrt(x);
}

#endif  // INCLUDE_FUNCTIONS_HPP