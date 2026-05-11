#include "networks.hpp"
#include "printer.hpp"
#include "simulator.hpp"

#include <iostream>

using stochastic::Printer;
using stochastic::Simulator;
using examples::make_circadian;

int main()
{
    const auto v = make_circadian();
    Printer printer{std::cout};
    v.accept(printer);
    Simulator sim{v, 42};
    sim.simulate(48.0);  // 48 hours, matches Figure 2
    return 0;
}
