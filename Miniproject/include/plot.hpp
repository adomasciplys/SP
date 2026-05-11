#ifndef MINIPROJECT_PLOT_HPP
#define MINIPROJECT_PLOT_HPP

#include <cstddef>
#include <string>
#include <vector>

namespace plot
{

// One labeled line in a multi-series line plot.
// `y` must align with the `times` vector passed to save_trajectory_plot
// y[i] is the value at times[i].
struct PlotSeries
{
    std::string name;
    std::vector<double> y;
};

// Render a multi-series line plot to a PNG using Qt Charts.
// If the trajectory has more than `max_points` samples, it is uniformly
// downsampled to `max_points` before rendering
void save_trajectory_plot(const std::string& filename,
                          const std::string& title,
                          const std::string& xlabel,
                          const std::string& ylabel,
                          const std::vector<double>& times,
                          const std::vector<PlotSeries>& series,
                          std::size_t max_points = 10000);

}  // namespace plot

#endif  // MINIPROJECT_PLOT_HPP
