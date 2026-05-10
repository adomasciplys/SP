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

int main()
{
    const auto v = seihr(10000);
    Printer{std::cout}.visit(v);

    // Stream the trajectory and track only the peak hospitalization
    Simulator sim{v, 42};
    const auto h_idx = v.find_index("H");
    std::size_t peak_H = 0;
    for (const auto& s : sim.run(100.0))
        peak_H = std::max(peak_H, s.counts[h_idx]);
    std::cerr << "Peak hospitalization (N=10000): " << peak_H << '\n';
    return 0;
}
