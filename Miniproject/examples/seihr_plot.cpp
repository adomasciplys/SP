#include "networks.hpp"
#include "plot.hpp"
#include "simulator.hpp"

#include <cstddef>
#include <vector>

using stochastic::PlotSeries;
using stochastic::Simulator;
using stochastic::examples::make_seihr;
using stochastic::save_trajectory_plot;

int main()
{
    const auto v = make_seihr(10000);
    Simulator sim{v, 42};

    std::vector<double> times;
    std::vector<double> S, E, I, H_scaled, R;
    for (const auto& s : sim.run(100.0)) {
        times.push_back(s.time);
        S.push_back(static_cast<double>(s.counts[0]));
        E.push_back(static_cast<double>(s.counts[1]));
        I.push_back(static_cast<double>(s.counts[2]));
        // Scale H by 1000 — its absolute count peaks ~2 at N=10000, otherwise
        // invisible against S which peaks near N. Matches the assignment's
        // Figure 3 ("H*1000" legend entry).
        H_scaled.push_back(static_cast<double>(s.counts[3]) * 1000.0);
        R.push_back(static_cast<double>(s.counts[4]));
    }

    save_trajectory_plot("figures/seihr_n10000.png",
                         "COVID-19 SEIHR (N=10000)",
                         "time, days",
                         "population count",
                         times,
                         {{"S", S}, {"E", E}, {"I", I},
                          {"H * 1000", H_scaled}, {"R", R}});
    return 0;
}
