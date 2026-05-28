#include "networks.hpp"   // examples::make_circadian — builds the circadian vessel
#include "plot.hpp"       // plot::collect_trajectory + save_trajectory_plot
#include "simulator.hpp"  // stochastic::Simulator

using plot::collect_trajectory;
using plot::save_trajectory_plot;
using stochastic::Simulator;
using examples::make_circadian;

int main()
{
    const auto v = make_circadian();
    Simulator sim{v, 42};

    // Drive the simulator for 48 hours
    // Ccollect every sample into one PlotSeries per species
    auto traj = collect_trajectory(v, sim, 48.0);

    // Wider image of 2400x800 so the 48-hour oscillations don't get crammed.
    save_trajectory_plot("figures/circadian.png",
                         "Circadian Rhythm",
                         "time, hours",
                         "molecule count",
                         traj.times, traj.series,
                         10000,
                         2400,
                         800);
    return 0;
}
