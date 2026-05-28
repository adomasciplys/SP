#include "plot.hpp"

#include <QApplication>
#include <QGraphicsScene>
#include <QImage>
#include <QPainter>
#include <QString>
#include <QtCharts/QChart>
#include <QtCharts/QLegend>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include <cstddef>
#include <vector>

namespace plot
{

namespace
{

// Qt requires exactly one QApplication per process
// QApplication is built the first time a plot is rendered and reused on every subsequent call
void ensure_qapplication()
{
    if (QCoreApplication::instance()) return;
    // QApplication's constructor takes argc and argv by reference
    static int argc = 0;
    static char* argv0 = nullptr;
    static char** argv = &argv0;
    static QApplication app{argc, argv};
}

// Pick at most `max` indices from [0, n) at uniform stride.
// Used to thin out very long trajectories before plotting
std::vector<std::size_t> stride_indices(std::size_t n, std::size_t max)
{
    std::vector<std::size_t> idx;
    if (n == 0) return idx;
    // Nothing to thin: hand back every index.
    if (n <= max) {
        idx.reserve(n);
        for (std::size_t i = 0; i < n; ++i) idx.push_back(i);
        return idx;
    }
    // Otherwise step through [0, n) in `max` equally-spaced jumps.
    // The last index is always n-1 (so the plot reaches the final sample
    idx.reserve(max);
    const double step = static_cast<double>(n - 1) / static_cast<double>(max - 1);
    for (std::size_t i = 0; i < max; ++i)
        idx.push_back(static_cast<std::size_t>(i * step));
    return idx;
}

}  // namespace

void save_trajectory_plot(const std::string& filename,
                          const std::string& title,
                          const std::string& xlabel,
                          const std::string& ylabel,
                          const std::vector<double>& times,
                          const std::vector<PlotSeries>& series,
                          std::size_t max_points,
                          int width,
                          int height)
{
    // Need a live QApplication before constructing any Qt widget/chart.
    ensure_qapplication();

    // Decide which samples to actually plot (caps long trajectories).
    const auto idx = stride_indices(times.size(), max_points);

    // Build the chart
    auto* chart = new QChart;
    chart->setTitle(QString::fromStdString(title));
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);

    // One coloured line per species.
    // Qt assigns each line a distinct colour from its default palette automatically.
    for (const auto& s : series) {
        auto* line = new QLineSeries;
        line->setName(QString::fromStdString(s.name));
        for (std::size_t k : idx) {
            if (k < s.y.size())
                line->append(times[k], s.y[k]);
        }
        chart->addSeries(line);
    }

    // Auto-fit X and Y axes to the data, then label them.
    chart->createDefaultAxes();
    if (auto axes = chart->axes(Qt::Horizontal); !axes.isEmpty())
        axes.first()->setTitleText(QString::fromStdString(xlabel));
    if (auto axes = chart->axes(Qt::Vertical); !axes.isEmpty())
        axes.first()->setTitleText(QString::fromStdString(ylabel));

    // Render via a QGraphicsScene at the caller-chosen size.
    chart->resize(width, height);
    QGraphicsScene scene;
    scene.addItem(chart);

    // Paint the scene into a white-backed image, then write it out as PNG.
    QImage img{width, height, QImage::Format_ARGB32};
    img.fill(Qt::white);
    QPainter painter{&img};
    painter.setRenderHint(QPainter::Antialiasing);
    scene.render(&painter);
    img.save(QString::fromStdString(filename));
}

}  // namespace plot
