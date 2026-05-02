#include "vessel.hpp"

#include <cstddef>
#include <iostream>

using stochastic::Vessel;

// Genetic oscillator for circadian rhythm - Listing 1
static Vessel circadian_rhythm()
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
    dump(circadian_rhythm());
    return 0;
}
