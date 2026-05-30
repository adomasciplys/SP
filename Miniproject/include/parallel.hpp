#ifndef MINIPROJECT_PARALLEL_HPP
#define MINIPROJECT_PARALLEL_HPP

#include "simulator.hpp"
#include "thread-pool.hpp"
#include <type_traits>
#include <vector>


// Reduce is the function supplied to reduce one finished simulation down to a single result.
// Basically, this template says:
//     - Reduce must be an invocable (e.g. a function)
//     - It also must take a Simulator as an argument
// Note: An AI assistant drafted the template declaration and the return type logic below.
// I understand how the return type works.
template <std::invocable<stochastic::Simulator&> Reduce>

// All simulators share the same vessel and run n simulations
// The results from each simulation are returned into a vector
// num_threads caps the worker pool
// Return a vector of whatever reduce(sim) produces, one entry per simulation run.
std::vector<std::invoke_result_t<Reduce, stochastic::Simulator&>> parallel_runs(const stochastic::Vessel& vessel,
                   std::size_t n, // number of simulations
                   std::size_t base_seed,
                   Reduce reduce, // function to aggregate simulator results
                   std::size_t num_threads = std::thread::hardware_concurrency() // number of workers
)
{
    using Result = std::invoke_result_t<Reduce, stochastic::Simulator&>; // shorten, so I do not have to repeat
    auto pool = thread_pool(num_threads); // capped worker count, so we never over-subscribe
    auto futures = std::vector<std::future<Result>>{}; // one future per run, kept in run order
    futures.reserve(n);

    // One simulation: build a Simulator with seed (base_seed + i), reduce its result.
    auto run_one = [&vessel, base_seed, &reduce](std::size_t i) {
        auto sim = stochastic::Simulator{vessel, base_seed + i};
        return reduce(sim); // reduce it to one value
    };

    // Queue n independent simulations.
    // Each run gets a distinct seed so the runs differ but stay reproducible
    for (std::size_t i = 0; i < n; ++i)
        futures.push_back(pool.async(run_one, i));

    // Block until every run finishes, then collect their results into a vector.
    return collect(std::move(futures));
}

#endif //MINIPROJECT_PARALLEL_HPP
