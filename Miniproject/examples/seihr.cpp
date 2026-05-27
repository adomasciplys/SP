#include "networks.hpp"
#include "printer.hpp"
#include "simulator.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <parallel.hpp>

using stochastic::Printer;
using stochastic::Simulator;
using examples::make_seihr;

// Build the network for the given population, stream the trajectory
// and keep only the peak hospitalization.
static std::size_t peak_hospitalization(std::uint32_t N, std::size_t seed)
{
    const auto v = make_seihr(N);
    Simulator sim{v, seed};
    const auto h_idx = v.find_index("H");
    std::size_t peak = 0;
    for (const auto& s : sim.run(100.0))
        peak = std::max(peak, s.counts[h_idx]);
    return peak;
}

static double mean_peak_hospitilzations(std::uint32_t population, std::size_t simulations, std::size_t seed)
{
    const auto vessel = make_seihr(population);
    auto peaks = parallel_runs(vessel , simulations, seed, [&vessel](Simulator& sim)
    {
        const auto h_index = vessel.find_index("H");
        std::size_t peak = 0;
        for (const auto& s : sim.run(100.0)) peak = std::max(peak, s.counts[h_index]);
        return peak;
    });
    double mean = std::accumulate(peaks.begin(), peaks.end(), 0.0) / peaks.size();
    return mean;
}


int main()
{
    // Print the network once — its shape is the same for every N.
    // Printer printer{std::cout};
    // make_seihr(10000).accept(printer);

    // Peaks go to stderr so stdout stays a clean dot stream.
    //std::cerr << "Peak hospitalization (N=10000):       "
    //   << peak_hospitalization(10'000, 42) << '\n';
    //std::cerr << "Peak hospitalization (N_NJ=589,755):   "
    //    << peak_hospitalization(589'755, 42) << '\n';
    //std::cerr << "Peak hospitalization (N_DK=5,822,763): "
    //    << peak_hospitalization(5'822'763, 42) << '\n';
    std::cerr << "Mean peak hospitilization after 100 runs" << mean_peak_hospitilzations(10'000, 100, 42) << '\n';
    return 0;
}
