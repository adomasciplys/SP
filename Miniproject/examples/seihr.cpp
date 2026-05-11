#include "networks.hpp"
#include "printer.hpp"
#include "simulator.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>

using stochastic::Printer;
using stochastic::Simulator;
using stochastic::examples::make_seihr;

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

int main()
{
    // Print the network once — its shape is the same for every N.
    Printer{std::cout}.visit(make_seihr(10000));

    // Peaks go to stderr so stdout stays a clean dot stream.
    std::cerr << "Peak hospitalization (N=10000):       "
              << peak_hospitalization(10'000, 42) << '\n';
    std::cerr << "Peak hospitalization (N_NJ=589,755):   "
              << peak_hospitalization(589'755, 42) << '\n';
    std::cerr << "Peak hospitalization (N_DK=5,822,763): "
              << peak_hospitalization(5'822'763, 42) << '\n';
    return 0;
}
