#ifndef TEMPLATES_FACTORIAL_HPP
#define TEMPLATES_FACTORIAL_HPP

#include <cstdint> // size_t

template <typename T = std::size_t>
constexpr T fact(std::size_t i)
{
	if (i < 2u)
		return T{1};
	else
		return fact<T>(i-1)*i;
}

template <std::size_t N, typename T=std::size_t>
struct Fact {
	static constexpr auto value = Fact<N-1,T>::value * N;
};

template <typename T>
struct Fact<0u, T> {
	static constexpr auto value = T{1};
};

#endif	// TEMPLATES_FIBONACCI_HPP
