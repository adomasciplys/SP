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
    Printer printer{std::cout};
    make_seihr(10000).accept(printer);

    std::cerr << "Peak hospitilization after 1 run" << mean_peak_hospitilzations(10'000, 1, 42) << '\n';
    std::cerr << "Peak hospitilization after 1 run" << mean_peak_hospitilzations(589'755, 1, 42) << '\n';
    std::cerr << "Peak hospitilization after 1 run" << mean_peak_hospitilzations(5'822'763, 1, 42) << '\n';
    std::cerr << "Mean peak hospitilization after 100 runs" << mean_peak_hospitilzations(5'822'763, 100, 42) << '\n';
    return 0;
}
