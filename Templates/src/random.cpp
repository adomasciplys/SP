#include "random.hpp"

#include <random>

/// Populates res with random numbers
void fill_random(std::span<double> res)
{
	auto gen  = std::mt19937_64{ 42 };
	auto dist = std::uniform_real_distribution<double>{ -1000, 1000 };
	for (auto& value : res)
		value = dist(gen);
}
