#include "networks.hpp"
#include "printer.hpp"
#include "simulator.hpp"

using stochastic::Printer;
using stochastic::Simulator;
using examples::make_circadian;

// Demonstrates DSL + Printer
int main()
{
    const auto v = make_circadian();
    Printer printer{"figures/circadian.dot"};
    v.accept(printer);
    return 0;
}
