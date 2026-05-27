#ifndef MINIPROJECT_PARALLEL_HPP
#define MINIPROJECT_PARALLEL_HPP

#include "simulator.hpp"
#include "thread-pool.hpp"

#include <cstddef>
#include <type_traits>
#include <vector>

// S must be something you can call with a stochastic::Simulator&
template <std::invocable<stochastic::Simulator&> Reduced>

// A function to run simulations in parallel
// All simulators share the same vessel and run n simulations
// The results from each simulation are returned into a vector
auto parallel_runs(const stochastic::Vessel& vessel, std::size_t n, std::size_t base_seed, Reduced reduced_result)
    -> std::vector<std::invoke_result_t<Reduced, stochastic::Simulator&>>
{
    auto pool = thread_pool{};
    using Result = std::invoke_result_t<Reduced, stochastic::Simulator&>;
    std::vector<std::future<Result>> futures;

    for (std::size_t i = 0; i < n; i++) {
        futures.push_back(pool.async([&vessel, base_seed, i, &reduced_result]() {
            stochastic::Simulator sim{vessel, base_seed + i};
            return reduced_result(sim);
        }));
    }

    return collect(std::move(futures));
}

#endif //MINIPROJECT_PARALLEL_HPP
