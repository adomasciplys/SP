#ifndef TEMPLATES_RANDOM_HPP
#define TEMPLATES_RANDOM_HPP

#include <span>
#include <vector>

/// Populates res with random numbers
void fill_random(std::span<double> res);

/// Creates a new vector with count random numbers
inline std::vector<double> make_random(std::size_t count = 100)
{
	auto res = std::vector<double>(count);
	fill_random(res);
	return res;
}

#endif	// TEMPLATES_RANDOM_HPP