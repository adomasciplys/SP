#include "measure.hpp"
#include "power.hpp"
#include "random.hpp"

#include <iostream>
#include <cmath>

int main()
{
	constexpr int ITERATIONS = 1'000'000; // Run each operation 1 million times

	// Test with base 2 and varying exponents
	for (auto exponent : { 5, 10, 15, 20, 25 }) {
		std::cout << std::string(40, '=') << std::endl;
		std::cout << "Computing 2^" << exponent << " (" << ITERATIONS << " iterations)" << std::endl;

		// Runtime power function
		{
			const auto [res, dur] = measure([exponent]() {
				volatile int result = 0;
				for (int i = 0; i < ITERATIONS; ++i) {
					result = power<int>(2, exponent);
				}
				return static_cast<int>(result);
			});
			std::cout << "runtime power: " << res << " in " << dur << std::endl;
		}

		// std::pow
		{
			const auto [res, dur] = measure([exponent]() {
				volatile double result = 0.0;
				for (int i = 0; i < ITERATIONS; ++i) {
					result = std::pow(2.0, static_cast<double>(exponent));
				}
				return static_cast<double>(result);
			});
			std::cout << "std::pow:      " << res << " in " << dur << std::endl;
		}

		// Compile-time power (for small exponents)
		if (exponent == 5) {
			const auto [res, dur] = measure([]() {
				volatile std::size_t result = 0;
				for (int i = 0; i < ITERATIONS; ++i) {
					result = Power<2, 5>::value;
				}
				return static_cast<std::size_t>(result);
			});
			std::cout << "compile-time:  " << res << " in " << dur << std::endl;
		}
		if (exponent == 10) {
			const auto [res, dur] = measure([]() {
				volatile std::size_t result = 0;
				for (int i = 0; i < ITERATIONS; ++i) {
					result = Power<2, 10>::value;
				}
				return static_cast<std::size_t>(result);
			});
			std::cout << "compile-time:  " << res << " in " << dur << std::endl;
		}
		if (exponent == 15) {
			const auto [res, dur] = measure([]() {
				volatile std::size_t result = 0;
				for (int i = 0; i < ITERATIONS; ++i) {
					result = Power<2, 15>::value;
				}
				return static_cast<std::size_t>(result);
			});
			std::cout << "compile-time:  " << res << " in " << dur << std::endl;
		}
		if (exponent == 20) {
			const auto [res, dur] = measure([]() {
				volatile std::size_t result = 0;
				for (int i = 0; i < ITERATIONS; ++i) {
					result = Power<2, 20>::value;
				}
				return static_cast<std::size_t>(result);
			});
			std::cout << "compile-time:  " << res << " in " << dur << std::endl;
		}
		if (exponent == 25) {
			const auto [res, dur] = measure([]() {
				volatile std::size_t result = 0;
				for (int i = 0; i < ITERATIONS; ++i) {
					result = Power<2, 25>::value;
				}
				return static_cast<std::size_t>(result);
			});
			std::cout << "compile-time:  " << res << " in " << dur << std::endl;
		}
	}
}