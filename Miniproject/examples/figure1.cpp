#include "networks.hpp"
#include "printer.hpp"
#include "simulator.hpp"

#include <cstddef>

using stochastic::Printer;
using stochastic::Simulator;
using examples::make_figure1;

static void run_config(std::size_t a0, std::size_t b0, std::size_t c0,
                       double end_time, std::size_t seed)
{
    auto v = make_figure1(a0, b0, c0);
    Simulator sim{v, seed};
    sim.simulate(end_time);
}

int main()
{
    // Dump the network structure to figures/figure1.dot (run `dot -Tpng` on it for an image).
    Printer printer{"figures/figure1.dot"};
    make_figure1(100, 0, 1).accept(printer);

    run_config(100,  0, 1, 2000.0, 42);
    run_config(100,  0, 2, 1500.0, 42);
    run_config( 50, 50, 1, 1500.0, 42);
    return 0;
}
