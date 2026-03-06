#ifndef TEMPLATES_FIBONACCI_HPP
#define TEMPLATES_FIBONACCI_HPP

#include <cstddef> // size_t
#include <array>

template <typename T = std::size_t>
T fib(std::size_t i)
{
	if (i < 2u)
		return T{1};
	else
		return fib<T>(i-1) + fib<T>(i-2);
}

template <size_t N>
struct Fib { // primary declaration: "induction step"
	static inline const size_t value = Fib<N-1>::value + Fib<N-2>::value; // Use inline to avoid linker conflicts
};

template <>
struct Fib<0> { // specialization: "base case 1"
	static inline const size_t value = 1;
};

template <>
struct Fib<1> { // specialization: "base case 2"
	static inline const size_t value = 1;
};

// Helper function that extracts Fib<I>::value for each index in the sequence
template <size_t... I>
constexpr auto fib_table_impl(std::index_sequence<I...>) {
	return std::array<size_t, sizeof...(I)>{Fib<I>::value...};
}

// User-facing function: creates a compile-time Fibonacci table of size N
template <size_t N>
constexpr auto fib_table() {
	return fib_table_impl(std::make_index_sequence<N>{});
}

constexpr auto table = fib_table<10>();

#endif	// TEMPLATES_FIBONACCI_HPP
