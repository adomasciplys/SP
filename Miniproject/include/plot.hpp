#ifndef MINIPROJECT_PLOT_HPP
#define MINIPROJECT_PLOT_HPP

#include <string>
#include <vector>

// Forward declarations, to not include Similator and everything with it
namespace stochastic { struct Vessel; struct Simulator; }

namespace plot
{

// One labeled line in a multi-series line plot.
// 'y' must align with the 'times' vector passed to save_trajectory_plot
// y[i] is the value at times[i].
struct PlotSeries
{
    std::string name;
    std::vector<double> y;
};

// A complete trajectory ready to hand to save_trajectory_plot.
// One PlotSeries per species, and a matching 'times' column.
// series[k] corresponds to v.species()[k].
struct Trajectory
{
    std::vector<double> times;
    std::vector<PlotSeries> series;
};

// Run simulation until end_time.
// Collect every sample into a Trajectory.
Trajectory collect_trajectory(const stochastic::Vessel& v,
                              stochastic::Simulator& sim,
                              double end_time);

// Render a multi-series line plot to a PNG using Qt Charts.
// All samples are drawn — fine for the trajectory sizes this project produces.
void save_trajectory_plot(const std::string& filename,
                          const std::string& title,
                          const std::string& xlabel,
                          const std::string& ylabel,
                          const std::vector<double>& times,
                          const std::vector<PlotSeries>& series,
                          int width  = 1200,
                          int height = 800);

}  // namespace plot

#endif  // MINIPROJECT_PLOT_HPP
