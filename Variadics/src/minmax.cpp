#include "minmax.hpp"

#include <iostream>
#include <algorithm>

std::ostream& operator<<(std::ostream& os, const MinMax& m)
{
	return os << "[" << m.min << ", " << m.max << "]";
}

MinMax if_cond(std::span<const double> values)
{
	auto res = MinMax{};
	for (const auto& value : values) {
		if (value < res.min)
			res.min = value;
		if (res.max < value)
			res.max = value;
	}
	return res;
}

MinMax minmax(std::span<const double> values)
{
	auto res = MinMax{};
	for (const auto& value : values) {
		res.min = std::min(res.min, value);
		res.max = std::max(res.max, value);
	}
	return res;
}

MinMax element(std::span<const double> values)
{
	const auto [min, max] = std::minmax_element(values.begin(), values.end());
	return MinMax{ *min, *max };
}

MinMax ranges(std::span<const double> values)
{
	const auto [min, max] = std::ranges::minmax_element(values);
	return MinMax{ *min, *max };
}
