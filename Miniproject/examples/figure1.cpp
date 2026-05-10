#include "printer.hpp"
#include "simulator.hpp"
#include "vessel.hpp"

#include <cstddef>
#include <iostream>

using stochastic::Printer;
using stochastic::Simulator;
using stochastic::Vessel;

// Figure 1 from the assignment: the toy reaction
//     A + C --(lambda)--> B + C
// C is a catalyst
// lambda = 0.001
static Vessel make_vessel(const char* label,
                          std::size_t a0, std::size_t b0, std::size_t c0)
{
    Vessel v{label};
    const auto A = v.add("A", a0);
    const auto B = v.add("B", b0);
    const auto C = v.add("C", c0);
    v.add((A + C) >> 0.001 >>= B + C);
    return v;
}

static void run_config(const char* label,
                       std::size_t a0, std::size_t b0, std::size_t c0,
                       double end_time, std::size_t seed)
{
    Vessel v = make_vessel(label, a0, b0, c0);
    Simulator sim{v, seed};
    sim.simulate(end_time);
}

int main()
{
    // Print the network structure once — all three configs share the same one.
    Printer{std::cout}.visit(make_vessel("figure1", 100, 0, 1));

    run_config("A100_B0_C1",  100,  0, 1, 2000.0, 42);
    run_config("A100_B0_C2",  100,  0, 2, 1500.0, 42);
    run_config("A50_B50_C1",   50, 50, 1, 1500.0, 42);
    return 0;
}
