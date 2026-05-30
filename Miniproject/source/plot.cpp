#include "plot.hpp"

#include "simulator.hpp"

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

// Qt requires exactly one QApplication per process: https://doc.qt.io/qt-6/qapplication.html
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

}  // namespace

// One labelled series per species
// Each Sample is appended to its column and discarded
Trajectory collect_trajectory(const stochastic::Vessel& v,
                              stochastic::Simulator& sim,
                              double end_time)
{
    Trajectory result;
    result.series.reserve(v.species().size());
    for (const auto& s : v.species())
        result.series.push_back({s.name, {}});

    for (const auto& sample : sim.run(end_time)) {
        result.times.push_back(sample.time);
        for (std::size_t k = 0; k < result.series.size(); ++k)
            result.series[k].y.push_back(sample.counts[k]);
    }
    return result;
}


// Note: The drawing logic of this function was drafted using an AI assistant
// to save time on reading Qt documentation
// The code has been reviewed. I assume full responsibility for its content.
void save_trajectory_plot(const std::string& filename,
                          const std::string& title,
                          const std::string& xlabel,
                          const std::string& ylabel,
                          const std::vector<double>& times,
                          const std::vector<PlotSeries>& series,
                          int width,
                          int height)
{
    // Need a live QApplication before constructing any Qt widget/chart.
    ensure_qapplication();

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
        for (std::size_t k = 0; k < times.size() && k < s.y.size(); ++k)
            line->append(times[k], s.y[k]);
        chart->addSeries(line);
    }

    // Auto-fit X and Y axes to the data, then label them.
    chart->createDefaultAxes();
    if (auto axes = chart->axes(Qt::Horizontal); !axes.isEmpty())
        axes.first()->setTitleText(QString::fromStdString(xlabel));
    if (auto axes = chart->axes(Qt::Vertical); !axes.isEmpty())
        axes.first()->setTitleText(QString::fromStdString(ylabel));

    // Render via a QGraphicsScene, at chosen size
    chart->resize(width, height);
    QGraphicsScene scene;
    scene.addItem(chart);

    // Paint the scene into white background image, then write it out as PNG.
    QImage img{width, height, QImage::Format_ARGB32};
    img.fill(Qt::white);
    QPainter painter{&img};
    painter.setRenderHint(QPainter::Antialiasing);
    scene.render(&painter);
    img.save(QString::fromStdString(filename));
}

}  // namespace plot
