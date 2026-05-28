#ifndef MINIPROJECT_PARALLEL_HPP
#define MINIPROJECT_PARALLEL_HPP

#include "simulator.hpp"
#include "thread-pool.hpp"
#include <type_traits>    // std::invoke_result_t: Deduces what `reduced_result` returns
#include <vector>

// Reduced is the function you supply to boil one finished simulation down to a single result.
template <std::invocable<stochastic::Simulator&> Reduced>

// All simulators share the same vessel and run n simulations
// The results from each simulation are returned into a vector
// num_threads caps the worker pool
auto parallel_runs(const stochastic::Vessel& vessel, std::size_t n, std::size_t base_seed, Reduced reduced_result,
                   std::size_t num_threads = std::thread::hardware_concurrency())
    // Return a vector of whatever reduced_result(sim) produces, one entry per run.
    -> std::vector<std::invoke_result_t<Reduced, stochastic::Simulator&>>
{
    auto pool = thread_pool(num_threads);  // capped worker count, so we never over-subscribe
    using Result = std::invoke_result_t<Reduced, stochastic::Simulator&>;  // per-run result type
    std::vector<std::future<Result>> futures;  // one future per run, kept in run order

    // Queue n independent simulations. Each run gets a distinct seed
    // so the runs differ but stay reproducible no matter which thread runs which.
    for (std::size_t i = 0; i < n; i++) {
        futures.push_back(pool.async([&vessel, base_seed, i, &reduced_result]() {
            stochastic::Simulator sim{vessel, base_seed + i};
            return reduced_result(sim);  // reduce it to one value
        }));
    }

    // Block until every run finishes, then collect their results into a vector.
    return collect(std::move(futures));
}

#endif //MINIPROJECT_PARALLEL_HPP
