#include "networks.hpp"   // examples::make_seihr — builds the COVID-19 SEIHR vessel
#include "plot.hpp"       // plot::collect_trajectory + save_trajectory_plot
#include "simulator.hpp"  // stochastic::Simulator

#include <cstddef>

using plot::collect_trajectory;
using plot::save_trajectory_plot;
using stochastic::Simulator;
using examples::make_seihr;

int main()
{
    const auto v = make_seihr(10'000);
    Simulator sim{v, 42};

    // Collect every sample into one PlotSeries per species (S, E, I, H, R).
    auto traj = collect_trajectory(v, sim, 100.0);

    // H peaks at single digits while S/E/I/R reach into the thousands,
    // so the H curve would be invisible on a shared axis.
    // Scale H by 1000 in-place and rename the legend label so the figure isn't misleading.
    const auto h_idx = v.find_index("H");
    constexpr double h_scale = 1000.0;
    traj.series[h_idx].name += " * 1000";
    for (auto& y : traj.series[h_idx].y) y *= h_scale;

    save_trajectory_plot("figures/seihr_n10000.png",
                         "COVID-19 SEIHR (N=10000)",
                         "time, days",
                         "population count",
                         traj.times, traj.series,
                         10000,2400, 800);
    return 0;
}
