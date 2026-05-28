#include "networks.hpp"
#include "parallel.hpp"
#include "simulator.hpp"

#include <benchmark/benchmark.h>

#include <algorithm>
#include <cstddef>
#include <thread>

using examples::make_seihr;
using stochastic::Simulator;

namespace
{
    // Population to simulate
    constexpr std::uint32_t kPopulation = 100'000;

    // 100 independent simulations.
    constexpr std::size_t kRuns = 100;

    // Run a single SEIHR trajectory and return its peak H count.
    std::size_t peak_hospitalization(Simulator& sim, std::size_t h_index)
    {
        std::size_t peak = 0;
        for (const auto& s : sim.run(100.0))
            peak = std::max(peak, s.counts[h_index]);
        return peak;
    }

    // Ensure we always use one thread
    unsigned max_threads() { return std::max(1u, std::thread::hardware_concurrency()); }
}

// Baseline: the 100 simulations run on one thread, no pool involved.
// This is the "single core" reference the parallel version is measured against.
static void bm_sequential(benchmark::State& state)
{
    const auto vessel = make_seihr(kPopulation);
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

// UseRealTime: report wall-clock time
BENCHMARK(bm_sequential)->Unit(benchmark::kMillisecond)->UseRealTime();

// Parallel: The same 100 simulations spread over worker threads.
//   range(0) == 1:           single core (bm_sequential, plus a little pool overhead)
//   range(0) == #cores:      multi-core
static void bm_parallel(benchmark::State& state)
{
    const auto threads = static_cast<std::size_t>(state.range(0));
    const auto vessel = make_seihr(kPopulation);
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

// Sweep 1, 2, 4, ... up to the core count so the JSON/plot shows the scaling curve.
BENCHMARK(bm_parallel)
    ->RangeMultiplier(2)->Range(1, max_threads())
    ->Unit(benchmark::kMillisecond)->UseRealTime();

// main() is provided by benchmark::benchmark_main (linked in tests/CMakeLists.txt).
