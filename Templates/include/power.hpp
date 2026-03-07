#ifndef TEMPLATES_POWER_HPP
#define TEMPLATES_POWER_HPP

#include "fibonacci.hpp"

#include <cstddef>
#include <type_traits>

template <typename T>
T power(T number, std::size_t p)
{
	if (p == 0)
		return 1;
	if (p == 1)
		return number;
	T res = power(number, p/2);
	res *= res;
	if (p%2 == 1)
		res *= number;
	return res;
}

template <std::size_t Base, std::size_t P>
struct Power
{
	static inline constexpr std::size_t value = Power<Base, P/2>::value * Power<Base, P/2>::value * (P % 2 == 1 ? Base : 1);
};

template <std::size_t Base>
struct Power<Base, 0>
{
	static inline constexpr std::size_t value = 1;
};

template <std::size_t Base>
struct Power<Base, 1>
{
	static inline constexpr std::size_t value = Base;
};


#endif	// TEMPLATES_POWER_HPP
