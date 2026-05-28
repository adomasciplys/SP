#include "networks.hpp"
#include "plot.hpp"
#include "simulator.hpp"

#include <array>
#include <cstddef>
#include <string>
#include <vector>

using plot::PlotSeries;
using plot::save_trajectory_plot;
using stochastic::Simulator;
using examples::make_figure1;

// Build the Figure 1 network at the given initial counts, simulate it, save the plot.
static void run_and_plot(const std::string& filename, const std::string& title,
                         std::size_t a0, std::size_t b0, std::size_t c0,
                         double end_time, std::size_t seed)
{
    const auto v = make_figure1(a0, b0, c0);
    Simulator sim{v, seed};

    // One labelled series per species. series[k].y[i] corresponds to v.species()[k] at time times[i]
    std::vector<PlotSeries> series;
    series.reserve(v.species().size());
    for (const auto& s : v.species())
        series.push_back({s.name, {}});

    // Stream the trajectory lazily via the Generator coroutine
    // Each Sample is appended to its column and then discarded
    std::vector<double> times;
    for (const auto& sample : sim.run(end_time)) {
        times.push_back(sample.time);
        for (std::size_t k = 0; k < series.size(); ++k)
            series[k].y.push_back(static_cast<double>(sample.counts[k]));
    }

    save_trajectory_plot(filename, title, "time", "count", times, series);
}

int main()
{
    // The three Figure 1 configurations from the assignment.
    // Each varies the catalyst count C and the initial A/B split
    struct Config {
        std::string filename;
        std::string title;
        std::size_t a0, b0, c0;
        double end_time;
    };

    const std::array configs{
        Config{"figures/figure1_a100_b0_c1.png", "A(0)=100, B(0)=0, C=1", 100,  0, 1, 2000.0},
        Config{"figures/figure1_a100_b0_c2.png", "A(0)=100, B(0)=0, C=2", 100,  0, 2, 1500.0},
        Config{"figures/figure1_a50_b50_c1.png", "A(0)=50,  B(0)=50, C=1",  50, 50, 1, 1500.0},
    };

    constexpr std::size_t seed = 42;  // fixed seed
    for (const auto& cfg : configs)
        run_and_plot(cfg.filename, cfg.title, cfg.a0, cfg.b0, cfg.c0, cfg.end_time, seed);
    return 0;
}
