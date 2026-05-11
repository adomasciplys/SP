#include "networks.hpp"
#include "plot.hpp"
#include "simulator.hpp"

#include <cstddef>
#include <string>
#include <vector>

using plot::PlotSeries;
using plot::save_trajectory_plot;
using stochastic::Simulator;
using stochastic::Vessel;
using examples::make_figure1;

static void run_and_plot(const std::string& filename, const std::string& title,
                         std::size_t a0, std::size_t b0, std::size_t c0,
                         double end_time, std::size_t seed)
{
    auto v = make_figure1(a0, b0, c0);
    Simulator sim{v, seed};

    std::vector<double> times, A, B, C;
    for (const auto& s : sim.run(end_time)) {
        times.push_back(s.time);
        A.push_back(static_cast<double>(s.counts[0]));
        B.push_back(static_cast<double>(s.counts[1]));
        C.push_back(static_cast<double>(s.counts[2]));
    }

    save_trajectory_plot(filename, title, "time", "count",
                         times, {{"A", A}, {"B", B}, {"C", C}});
}

int main()
{
    run_and_plot("figures/figure1_a100_b0_c1.png",
                 "A(0)=100, B(0)=0, C=1",
                 100, 0, 1, 2000.0, 42);
    run_and_plot("figures/figure1_a100_b0_c2.png",
                 "A(0)=100, B(0)=0, C=2",
                 100, 0, 2, 1500.0, 42);
    run_and_plot("figures/figure1_a50_b50_c1.png",
                 "A(0)=50, B(0)=50, C=1",
                 50, 50, 1, 1500.0, 42);
    return 0;
}
