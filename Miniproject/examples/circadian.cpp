#include "networks.hpp"
#include "printer.hpp"
#include "simulator.hpp"

using stochastic::Printer;
using stochastic::Simulator;
using examples::make_circadian;

int main()
{
    const auto v = make_circadian();
    // Dump the network structure to figures/circadian.dot.
    Printer printer{"figures/circadian.dot"};
    v.accept(printer);
    Simulator sim{v, 42};
    sim.simulate(48.0);  // 48 hours, matches Figure 2
    return 0;
}
