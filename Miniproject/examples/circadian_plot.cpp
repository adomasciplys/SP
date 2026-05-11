#include "networks.hpp"
#include "plot.hpp"
#include "simulator.hpp"

#include <cstddef>
#include <string>
#include <vector>

using stochastic::PlotSeries;
using stochastic::Simulator;
using stochastic::examples::make_circadian;
using stochastic::save_trajectory_plot;

int main()
{
    const auto v = make_circadian();
    Simulator sim{v, 42};

    const std::vector<std::string> names =
        {"DA", "D_A", "DR", "D_R", "MA", "MR", "A", "R", "C"};

    std::vector<double> times;
    std::vector<std::vector<double>> ys(names.size());
    for (const auto& s : sim.run(48.0)) {
        times.push_back(s.time);
        for (std::size_t k = 0; k < names.size(); ++k)
            ys[k].push_back(static_cast<double>(s.counts[k]));
    }

    std::vector<PlotSeries> series;
    series.reserve(names.size());
    for (std::size_t k = 0; k < names.size(); ++k)
        series.push_back({names[k], std::move(ys[k])});

    save_trajectory_plot("figures/circadian.png",
                         "Circadian Rhythm",
                         "time, hours",
                         "molecule count",
                         times, series);
    return 0;
}
