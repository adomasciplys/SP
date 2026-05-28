#include "networks.hpp"   // examples::make_seihr — builds the SEIHR vessel
#include "printer.hpp"    // stochastic::Printer — Graphviz dot output
#include "simulator.hpp"  // stochastic::Simulator

#include <algorithm>   // std::max
#include <cstddef>     // std::size_t
#include <cstdint>     // std::uint32_t
#include <iostream>    // std::cout
#include <parallel.hpp>  // parallel_runs — runs many simulations across cores

using stochastic::Printer;
using stochastic::Simulator;
using examples::make_seihr;


// Run `simulations` independent SEIHR runs for the given population
// and return the mean of their peak hospitalization.
static double mean_peak_hospitilzations(std::uint32_t population, std::size_t simulations, std::size_t seed)
{
    const auto vessel = make_seihr(population);  // one shared, read-only network for all runs
    // parallel_runs gives each run its own Simulator; this lambda reduces a run to its peak H.
    auto peaks = parallel_runs(vessel , simulations, seed, [&vessel](Simulator& sim)
    {
        const auto h_index = vessel.find_index("H");  // position of "H" in the counts vector
        std::size_t peak = 0;
        // Keep only the largest H seen
        for (const auto& s : sim.run(100.0)) peak = std::max(peak, s.counts[h_index]);
        return peak;
    });
    // Average the per-run peaks
    double mean = std::accumulate(peaks.begin(), peaks.end(), 0.0) / peaks.size();
    return mean;
}


int main()
{
    Printer printer{"figures/seihr.dot"};
    make_seihr(10000).accept(printer);

    // Single run (simulations = 1) at three population scales (Exercise 7)
    std::cout << "Peak hospitilization after 1 run - Population = 10.000:  " << mean_peak_hospitilzations(10'000, 1, 42) << '\n';
    std::cout << "Peak hospitilization after 1 run - Population = 589.755:  " << mean_peak_hospitilzations(589'755, 1, 42) << '\n';
    std::cout << "Peak hospitilization after 1 run - Population = 5.822.763:  "<< mean_peak_hospitilzations(5'822'763, 1, 42) << '\n';

    // Mean peak hospitalization estimated over 100 parallel runs (Exercise 9)
    std::cout << "Mean peak hospitilization after 100 runs - Population 10.000:  " << mean_peak_hospitilzations(10'000, 100, 42) << '\n';
    std::cout << "Mean peak hospitilization after 100 runs - Population 589.755:  "<< mean_peak_hospitilzations(589'755, 100, 42) << '\n';
    std::cout << "Mean peak hospitilization after 100 runs - Population 5.822.763:  " << mean_peak_hospitilzations(5'822'763, 100, 42) << '\n';
    return 0;
}
