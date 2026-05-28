#include "networks.hpp"   // examples::make_seihr — builds the COVID-19 SEIHR vessel
#include "plot.hpp"       // plot::PlotSeries + save_trajectory_plot (Qt6 + QtCharts)
#include "simulator.hpp"  // stochastic::Simulator

#include <cstddef>
#include <string>
#include <vector>

using plot::PlotSeries;
using plot::save_trajectory_plot;
using stochastic::Simulator;
using examples::make_seihr;

int main()
{
    const auto v = make_seihr(10'000);
    Simulator sim{v, 42};

    // H peaks at single digits while S/E/I/R reach into the thousands,
    // so the H curve would be invisible on a shared axis.
    // Scale H by 1000 just for the plot
    const auto h_idx = v.find_index("H");
    constexpr double h_scale = 1000.0;

    // One labelled series per species
    // The H series gets a " * 1000" suffix so the scaling is visible in the legend.
    std::vector<PlotSeries> series;
    series.reserve(v.species().size());
    for (std::size_t k = 0; k < v.species().size(); ++k) {
        const auto& s = v.species()[k];
        series.push_back({k == h_idx ? s.name + " * 1000" : s.name, {}});
    }

    // Stream the trajectory lazily via the Generator coroutine
    // Each Sample is appended (with H scaled) and then discarded
    std::vector<double> times;
    for (const auto& sample : sim.run(100.0)) {
        times.push_back(sample.time);
        for (std::size_t k = 0; k < series.size(); ++k) {
            const double y = static_cast<double>(sample.counts[k]);
            series[k].y.push_back(k == h_idx ? y * h_scale : y);
        }
    }

    save_trajectory_plot("figures/seihr_n10000.png",
                         "COVID-19 SEIHR (N=10000)",
                         "time, days",
                         "population count",
                         times, series);
    return 0;
}
