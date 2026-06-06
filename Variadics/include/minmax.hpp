#ifndef TEMPLATES_MINMAX_HPP
#define TEMPLATES_MINMAX_HPP

#include <limits>
#include <iosfwd>
#include <span>

/// Captures min and max
struct MinMax
{
	double				 min = std::numeric_limits<double>::max();
	double				 max = std::numeric_limits<double>::lowest();
	friend std::ostream& operator<<(std::ostream& os, const MinMax& m);
};

/// Uses if-condition to find min and max
MinMax if_cond(std::span<const double> values);

/// Uses std::min and std::max to find min and max
MinMax minmax(std::span<const double> values);

/// Uses std::minmax_element to find min and max
MinMax element(std::span<const double> values);

/// Uses std::ranges::minmax_element to find min and max
MinMax ranges(std::span<const double> values);

#endif	// TEMPLATES_MINMAX_HPP