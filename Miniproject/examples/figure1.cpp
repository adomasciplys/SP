#include "networks.hpp"
#include "printer.hpp"
#include "simulator.hpp"

#include <cstddef>

using stochastic::Printer;
using stochastic::Simulator;
using examples::make_figure1;

// Demonstrates DSL + Printer
int main()
{
    Printer printer{"figures/figure1.dot"};
    make_figure1(100, 0, 1).accept(printer);
    return 0;
}
