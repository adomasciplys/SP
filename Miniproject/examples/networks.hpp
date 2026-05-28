#ifndef MINIPROJECT_EXAMPLES_NETWORKS_HPP
#define MINIPROJECT_EXAMPLES_NETWORKS_HPP

#include "vessel.hpp"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <string>

namespace examples
{
using stochastic::Vessel;

// Figure 1 from the assignment
//     A + C -- (lambda) --> B + C
// C is a catalyst, lambda = 0.001.
inline Vessel make_figure1(std::size_t a0, std::size_t b0, std::size_t c0)
{
    Vessel v{"figure1"};
    const auto A = v.add("A", a0);
    const auto B = v.add("B", b0);
    const auto C = v.add("C", c0);
    v.add((A + C) >> 0.001 >>= B + C);
    return v;
}

// Figure 3 from assignment
// SEIHR model for COVID-19
inline Vessel make_seihr(std::uint32_t N)
{
    auto v = Vessel{"COVID19 SEIHR: " + std::to_string(N)};

    const auto eps = 0.0009; // initial fraction of infectious
    const auto I0 = static_cast<std::size_t>(std::round(eps * N)); // initial infectious
    const auto E0 = static_cast<std::size_t>(std::round(eps * N * 15)); // initial exposed
    const auto S0 = static_cast<std::size_t>(N) - I0 - E0; // initial susceptible
    const auto R0 = 2.4; // initial basic reproductive number
    const auto alpha = 1.0 / 5.1; // incubation rate (E -> I) ~5.1 days
    const auto gamma = 1.0 / 3.1; // recovery rate (I -> R) ~3.1 days
    const auto beta = R0 * gamma; // infection/generation rate (S+I -> E+I)
    const auto P_H = 0.9e-3; // probability of hospitalization
    const auto kappa = gamma * P_H * (1.0 - P_H); // hospitalization rate (I -> H)
    const auto tau = 1.0 / 10.12; // removal rate in hospital (H -> R)

    const auto S = v.add("S", S0); // susceptible
    const auto E = v.add("E", E0); // exposed
    const auto I = v.add("I", I0); // infectious
    const auto H = v.add("H", 0); // hospitalized
    const auto R = v.add("R", 0); // removed/immune (recovered + dead)

    v.add((S + I) >> beta / N >>= E + I); // susceptible becomes exposed by infectious
    v.add(E >> alpha >>= I); // exposed becomes infectious
    v.add(I >> gamma >>= R); // infectious becomes removed
    v.add(I >> kappa >>= H); // infectious becomes hospitalized
    v.add(H >> tau >>= R); // hospitalized becomes removed

    return v;
}

// Figure 2 from assignment
// Genetic oscillator for circadian rhythm
inline Vessel make_circadian()
{
    const auto alphaA = 50;
    const auto alpha_A = 500;
    const auto alphaR = 0.01;
    const auto alpha_R = 50;
    const auto betaA = 50;
    const auto betaR = 5;
    const auto gammaA = 1;
    const auto gammaR = 1;
    const auto gammaC = 2;
    const auto deltaA = 1;
    const auto deltaR = 0.2;
    const auto deltaMA = 10;
    const auto deltaMR = 0.5;
    const auto thetaA = 50;
    const auto thetaR = 100;

    auto v = Vessel{"Circadian Rhythm"};
    const auto env = v.environment();
    const auto DA = v.add("DA", 1);
    const auto D_A = v.add("D_A", 0);
    const auto DR = v.add("DR", 1);
    const auto D_R = v.add("D_R", 0);
    const auto MA = v.add("MA", 0);
    const auto MR = v.add("MR", 0);
    const auto A = v.add("A", 0);
    const auto R = v.add("R", 0);
    const auto C = v.add("C", 0);

    v.add((A + DA) >> gammaA >>= D_A);
    v.add(D_A >> thetaA >>= DA + A);
    v.add((A + DR) >> gammaR >>= D_R);
    v.add(D_R >> thetaR >>= DR + A);
    v.add(D_A >> alpha_A >>= MA + D_A);
    v.add(DA >> alphaA >>= MA + DA);
    v.add(D_R >> alpha_R >>= MR + D_R);
    v.add(DR >> alphaR >>= MR + DR);
    v.add(MA >> betaA >>= MA + A);
    v.add(MR >> betaR >>= MR + R);
    v.add((A + R) >> gammaC >>= C);
    v.add(C >> deltaA >>= R);
    v.add(A >> deltaA >>= env);
    v.add(R >> deltaR >>= env);
    v.add(MA >> deltaMA >>= env);
    v.add(MR >> deltaMR >>= env);
    return v;
}

}  // namespace examples

#endif  // MINIPROJECT_EXAMPLES_NETWORKS_HPP
