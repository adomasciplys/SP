#include "minmax.hpp"
#include "measure.hpp"
#include "random.hpp"

#include <iostream>

int main()
{
	for (auto count : { 10, 100, 1000, 10000, 1'000'000 }) {
		std::cout << std::string(40, '=') << std::endl;
		std::cout << "count: " << count << std::endl;
		const auto input = make_random(count);
		{
			const auto [res, dur] = measure(if_cond, input);
			std::cout << "if_cond: " << res << " in " << dur << std::endl;
		}
		{
			const auto [res, dur] = measure(minmax, input);
			std::cout << "minmax:  " << res << " in " << dur << std::endl;
		}
		{
			const auto [res, dur] = measure(element, input);
			std::cout << "element: " << res << " in " << dur << std::endl;
		}
		{
			const auto [res, dur] = measure(ranges, input);
			std::cout << "ranges:  " << res << " in " << dur << std::endl;
		}
	}
}