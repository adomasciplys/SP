#include "printer.hpp"
#include "vessel.hpp"

#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>

using stochastic::Printer;
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

static void dump(const Vessel& v)
{
    std::cout << "Vessel: " << v.name() << "\n";
    std::cout << "Species (" << v.species().size() << "):\n";
    for (const auto& s : v.species())
        std::cout << "  " << s.name << " = " << s.initial_count << "\n";

    std::cout << "Reactions (" << v.reactions().size() << "):\n";
    for (const auto& r : v.reactions()) {
        for (std::size_t i = 0; i < r.inputs.items.size(); ++i)
            std::cout << (i ? " + " : "") << r.inputs.items[i].name;
        std::cout << " --(" << r.rate << ")--> ";
        for (std::size_t i = 0; i < r.products.items.size(); ++i) {
            const auto& p = r.products.items[i];
            std::cout << (i ? " + " : "") << (p.is_environment() ? "env" : p.name);
        }
        std::cout << "\n";
    }
}

int main()
{
    const auto v = seihr(10000);
    dump(v);
    std::cout << "\n";
    Printer{std::cout}.visit(v);
    return 0;
}
