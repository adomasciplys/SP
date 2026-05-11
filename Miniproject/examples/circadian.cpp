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
    Printer{std::cout}.visit(v);
    Simulator sim{v, 42};
    sim.simulate(48.0);  // 48 hours, matches Figure 2
    return 0;
}
