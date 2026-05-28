#include "networks.hpp"   // examples::make_circadian — builds the circadian vessel
#include "plot.hpp"       // plot::PlotSeries + save_trajectory_plot (Qt6 + QtCharts)
#include "simulator.hpp"  // stochastic::Simulator

#include <vector>

using plot::PlotSeries;
using plot::save_trajectory_plot;
using stochastic::Simulator;
using examples::make_circadian;

int main()
{
    const auto v = make_circadian();
    Simulator sim{v, 42};

    // One labelled series per species.
    // series[k].y[i] corresponds to v.species()[k] at time times[i].
    std::vector<PlotSeries> series;
    series.reserve(v.species().size());
    for (const auto& s : v.species())
        series.push_back({s.name, {}});

    // Stream the trajectory lazily via the Generator coroutine.
    // Each Sample is consumed, appended to its column, then discarded.
    std::vector<double> times;
    for (const auto& sample : sim.run(48.0))
    {
        times.push_back(sample.time);
        for (std::size_t k = 0; k < series.size(); ++k)
            series[k].y.push_back(static_cast<double>(sample.counts[k]));
    }

    // Wider image of 2400x800 so the 48-hour oscillations don't get crammed.
    save_trajectory_plot("figures/circadian.png",
                         "Circadian Rhythm",
                         "time, hours",
                         "molecule count",
                         times, series,
                         10000,
                         2400,
                         800);
    return 0;
}
