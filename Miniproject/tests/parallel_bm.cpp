#include "networks.hpp"
#include "parallel.hpp"
#include "simulator.hpp"

#include <benchmark/benchmark.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>

using examples::make_seihr;
using stochastic::Simulator;

namespace
{
    // 100 independent simulations.
    constexpr std::size_t kRuns = 100;

    // Each benchmark function is re-run this many times so Google Benchmark
    // can report mean / median / stddev instead of a single sample.
    constexpr int kRepetitions = 5;

    // Run a single SEIHR trajectory and return its peak H count.
    std::size_t peak_hospitalization(Simulator& sim, std::size_t h_index)
    {
        std::size_t peak = 0;
        for (const auto& s : sim.run(100.0))
            peak = std::max(peak, s.counts[h_index]);
        return peak;
    }
}

// Baseline: 100 simulations on one thread, no pool involved.
static void bm_sequential(benchmark::State& state)
{
    const auto population = static_cast<std::uint32_t>(state.range(0));
    const auto vessel = make_seihr(population);
    const auto h = vessel.find_index("H");
    for (auto _ : state)
    {
        std::size_t total = 0;
        for (std::size_t i = 0; i < kRuns; ++i)
        {
            Simulator sim{vessel, 42 + i}; // distinct seed per run
            total += peak_hospitalization(sim, h);
        }
        benchmark::DoNotOptimize(total); // stop the compiler from discarding the work
        benchmark::ClobberMemory();
    }
}

// UseRealTime: report wall-clock time.
BENCHMARK(bm_sequential)
    ->Arg(10'000)->Arg(100'000)
    ->Unit(benchmark::kMillisecond)->UseRealTime()
    ->Repetitions(kRepetitions)->ReportAggregatesOnly(true);

// Parallel: The 100 simulations across:
//   threads == 1:                       single core (should match bm_sequential up to pool overhead).
//   threads == hardware_concurrency:    full multi-core
//   threads >  hardware_concurrency:    deliberate over-subscription.
static void bm_parallel(benchmark::State& state)
{
    const auto threads = static_cast<std::size_t>(state.range(0));
    const auto population = static_cast<std::uint32_t>(state.range(1));
    const auto vessel = make_seihr(population);
    const auto h = vessel.find_index("H");
    for (auto _ : state)
    {
        auto peaks = parallel_runs(vessel, kRuns, 42,
                                   [h](Simulator& sim) { return peak_hospitalization(sim, h); },
                                   threads);
        benchmark::DoNotOptimize(peaks);
        benchmark::ClobberMemory();
    }
}

// Threads x population
BENCHMARK(bm_parallel)
    ->ArgsProduct({{1, 2, 4, 8, 16, 32, 100}, {10'000, 100'000}})
    ->Unit(benchmark::kMillisecond)->UseRealTime()
    ->Repetitions(kRepetitions)->ReportAggregatesOnly(true);
