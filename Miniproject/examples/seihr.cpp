#include "printer.hpp"
#include "simulator.hpp"
#include "vessel.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>

using stochastic::Printer;
using stochastic::Simulator;
using stochastic::Vessel;

// SEIHR model for COVID-19 - Listing 2
static Vessel seihr(std::uint32_t N)
{
    auto v = Vessel{"COVID19 SEIHR: " + std::to_string(N)};

    const auto eps = 0.0009;
    const auto I0 = static_cast<std::size_t>(std::round(eps * N));
    const auto E0 = static_cast<std::size_t>(std::round(eps * N * 15));
    const auto S0 = static_cast<std::size_t>(N) - I0 - E0;
    const auto R0 = 2.4;
    const auto alpha = 1.0 / 5.1;
    const auto gamma = 1.0 / 3.1;
    const auto beta = R0 * gamma;
    const auto P_H = 0.9e-3;
    const auto kappa = gamma * P_H * (1.0 - P_H);
    const auto tau = 1.0 / 10.12;

    const auto S = v.add("S", S0);
    const auto E = v.add("E", E0);
    const auto I = v.add("I", I0);
    const auto H = v.add("H", 0);
    const auto R = v.add("R", 0);

    v.add((S + I) >> beta / N >>= E + I);
    v.add(E >> alpha >>= I);
    v.add(I >> gamma >>= R);
    v.add(I >> kappa >>= H);
    v.add(H >> tau >>= R);

    return v;
}

// Build the network for the given population, stream the trajectory
// and eep only the peak hospitalization.
static std::size_t peak_hospitalization(std::uint32_t N, std::size_t seed)
{
    const auto v = seihr(N);
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
    Printer{std::cout}.visit(seihr(10000));

    // Peaks go to stderr so stdout stays a clean dot stream.
    std::cerr << "Peak hospitalization (N=10000):       "
              << peak_hospitalization(10'000, 42) << '\n';
    std::cerr << "Peak hospitalization (N_NJ=589,755):   "
              << peak_hospitalization(589'755, 42) << '\n';
    std::cerr << "Peak hospitalization (N_DK=5,822,763): "
              << peak_hospitalization(5'822'763, 42) << '\n';
    return 0;
}
