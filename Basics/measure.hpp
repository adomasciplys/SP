/// Simple benchmarking library with statistics
/// Author: Marius Mikucionis <marius@cs.aau.dk>

#ifndef INCLUDE_MEASURE_HPP
#define INCLUDE_MEASURE_HPP

#include <chrono>
#include <map>
#include <vector>
#include <string>
#include <format>
#include <source_location>
#include <iostream>
#include <limits>

#include <cmath>

#if defined(_WIN32) || defined(_WIN64) || defined(__MINGW32__) || defined(__MINGW64__)
#ifndef NOMINMAX
# define NOMINMAX
#endif// NOMINMAX
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#endif // Windows (to fix unicode UTF8 output)

/// Stores counters and computes sample statistics
struct Statistics
{
    constexpr Statistics() = default;
    constexpr explicit Statistics(std::initializer_list<double> list)
    {
        for (auto& value : list)
            add(value);
    }
    /// Resets counters
    void reset()
    {
        _count = 0;
        _sum = 0;
        _sumsq = 0;
    }
    /// Adds a value to statistics
    constexpr Statistics& add(double value)
    {
        ++_count;
        _sum += value;
        _sumsq += value * value;
        return *this;
    }
    /// Number of values recorded in a sample
    constexpr std::size_t count() const { return _count; }
    constexpr double sum() const { return _sum; }
    constexpr double sumsq() const { return _sumsq; }
    constexpr double mean() const { return _sum / _count; }
    /// Corrected sample variance:
    constexpr double var() const
    {
        if (count() < 2)
            throw std::logic_error("Variance is undefined");
        return (sumsq() - sum() * sum() / count()) / (count() - 1);
    }
    /// Corrected sample standard deviation:
    double sd() const { return std::sqrt(var()); }
    /// Half width of 95% confidence interval
    double half_ci95() const
    {
        static constexpr double t[] = {
            12.706204736432, 4.302652729696, 3.182446305284, 2.776445105198, 2.570581835636, 2.446911851145, 2.364624251593, 2.306004135204, 2.262157162854, 2.228138851965, 2.200985160083, 2.178812829663, 2.160368656461, 2.144786687917, 2.131449545559, 2.119905299221, 2.109815577833, 2.100922040241, 2.093024054408, 2.085963447266, 2.079613844728, 2.073873067904, 2.068657610419, 2.063898561628, 2.059538552753, 2.055529438643, 2.051830516480, 2.048407141795, 2.045229642133, 2.042272456301, 2.039513446396, 2.036933343460, 2.034515297449, 2.032244509318, 2.030107928250, 2.028094000980, 2.026192463029, 2.024394163912, 2.022690920037, 2.021075390306, 2.019540970441, 2.018081702818, 2.016692199228, 2.015367574444, 2.014103388881, 2.012895598919, 2.011740513730, 2.010634757624, 2.009575237129, 2.008559112101, 2.007583770316, 2.006646805062, 2.005745995318, 2.004879288188, 2.004044783289, 2.003240718848, 2.002465459291, 2.001717484145, 2.000995378088, 2.000297822014, 1.999623584995, 1.998971517033, 1.998340542521, 1.997729654318, 1.997137908392, 1.996564418952, 1.996008354025, 1.995468931430, 1.994945415107, 1.994437111771, 1.993943367846, 1.993463566662, 1.992997125890, 1.992543495181, 1.992102154002, 1.991672609645, 1.991254395388, 1.990847068812, 1.990450210230, 1.990063421254, 1.989686323457, 1.989318557137, 1.988959780175, 1.988609666976, 1.988267907477, 1.987934206239, 1.987608281589, 1.987289864831, 1.986978699506, 1.986674540704, 1.986377154419, 1.986086316951, 1.985801814346, 1.985523441867, 1.985251003509, 1.984984311531, 1.984723186027, 1.984467454427, 1.984216951509};
		static constexpr auto SIZE = sizeof(t) / sizeof(t[0]);
        const auto index = (count()-2 < SIZE) ? (count() - 2) : (SIZE-1);
        return std::sqrt(var() / count()) * t[index];
    }
    double half_ci99() const
    {
        static constexpr double t[] = {63.656741162874, 9.924843200918, 5.840909309733, 4.604094871416, 4.032142983558, 3.707428021325, 3.499483297351, 3.355387331333, 3.249835541592, 3.169272672617, 3.105806515539, 3.054539589393, 3.012275838717, 2.976842734371, 2.946712883486, 2.920781622496, 2.898230519635, 2.878440472714, 2.860934606450, 2.845339709777, 2.831359558017, 2.818756060596, 2.807335683768, 2.796939504773, 2.787435813676, 2.778714533329, 2.770682957122, 2.763262455461, 2.756385903670, 2.749995653567, 2.744041919294, 2.738481482012, 2.733276642351, 2.728394367071, 2.723805589208, 2.719484630450, 2.715408721550, 2.711557601913, 2.707913183518, 2.704459267433, 2.701181303579, 2.698066186220, 2.695102079158, 2.692278265693, 2.689585019375, 2.687013492242, 2.684555617867, 2.682204026950, 2.679951973632, 2.677793270941, 2.675722234111, 2.673733630647, 2.671822636241, 2.669984795735, 2.668215988486, 2.666512397556, 2.664870482242, 2.663286953538, 2.661758752163, 2.660283028855, 2.658857126654, 2.657478564951, 2.656145025100, 2.654854337411, 2.653604469383, 2.652393515028, 2.651219685184, 2.650081298695, 2.648976774389, 2.647904623751, 2.646863444238, 2.645851913159, 2.644868782073, 2.643912871653, 2.642983066967, 2.642078313146, 2.641197611389, 2.640340015292, 2.639504627453, 2.638690596344, 2.637897113416, 2.637123410420, 2.636368756932, 2.635632458048, 2.634913852254, 2.634212309446, 2.633527229082, 2.632858038478, 2.632204191200, 2.631565165587, 2.630940463358, 2.630329608316, 2.629732145143, 2.629147638262, 2.628575670783, 2.628015843510, 2.627467774013, 2.626931095756, 2.626405457281};
		static constexpr auto SIZE = sizeof(t) / sizeof(t[0]);
        const auto index = (count()-2 < SIZE) ? (count() - 2) : (SIZE-1);
        return std::sqrt(var() / count()) * t[index];
    }

private:
    size_t _count{0}; ///< number of values
    double _sum{0}; ///< sum of values
    double _sumsq{0}; ///< sum of squared values
};

/// Manages time measurements
struct Measurements
{
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = Clock::time_point;
    using Duration = std::chrono::duration<double, std::micro>;
    /// Sample (benchmark) identifies
    struct SampleId
    {
        static constexpr auto INVALID = std::numeric_limits<std::size_t>::max();
        explicit SampleId(std::size_t value = INVALID) : _value{value} {}
        std::size_t value() const { return _value; }
        bool is_valid() const { return _value != INVALID; }

    private:
        std::size_t _value;
    };
    struct StopWatch
    {
        explicit StopWatch(Statistics& stats) : stats{stats}
        {
            started = Clock::now();
        }
        ~StopWatch()
        {
            const auto now = Clock::now();
            stats.add(Duration(now - started).count());
        }
        StopWatch(const StopWatch&) = delete;
        StopWatch& operator=(const StopWatch&) = delete;

    private:
        TimePoint started;
        Statistics& stats;
    };
    /// Preallocates the number of benchmarks
    Measurements(std::size_t count = 100)
    {
        started.reserve(count);
        stats.reserve(count);
#if defined(_WIN32) || defined(_WIN64) || defined(__MINGW32__) || defined(__MINGW64__)
        // Tell Windows console to use UTF8:
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
        // Tell C++ i/o to use UTF-8 mode:
        //_setmode(_fileno(stdout), _O_U8TEXT);
        //_setmode(_fileno(stdin),  _O_U8TEXT);
#endif // Windows
    }
    /// Finds a sample id by name:
    SampleId find(const std::string& name) const
    {
        if (auto it = names.find(name); it != names.end())
            return SampleId{it->second};
        return SampleId{};
    }
    /// Creates a sample with given name:
    SampleId create(const std::string& name)
    {
        if (auto id = find(name); id.is_valid())
            throw std::logic_error("duplicate name: " + name);
        return make(name);
    }
    SampleId create(std::source_location loc = std::source_location::current())
	{
		return create(std::format("{}:{}", loc.file_name(), loc.line()));
	}
    /// Starts a stopwatch for sample with given id:
    void start(SampleId id) { started[id.value()] = Clock::now(); }
    /// Stops the stopwatch and records its value:
    void stop(SampleId id)
    {
        const auto now = Clock::now();
        stats[id.value()].add(Duration(now - started[id.value()]).count());
    }
    /// Launches a stopwatch for the given sample id
    StopWatch launch(SampleId id) { return StopWatch{stats[id.value()]}; }
    StopWatch launch(const std::string& name)
    {
        auto id = find(name);
        if (!id.is_valid())
            id = make(name);
        return launch(id);
    }
    StopWatch launch(std::source_location loc = std::source_location::current())
    {
        return launch(std::format("{}:{}", loc.file_name(), loc.line()));
    }

private:
    std::map<std::string, std::size_t> names;
    std::vector<TimePoint> started;
    std::vector<Statistics> stats;
    SampleId make(std::string name)
    {
        auto res = stats.size();
        names.emplace(std::move(name), res);
        stats.emplace_back();
        started.emplace_back();
        return SampleId{res};
    }
    friend std::ostream& operator<<(std::ostream& os, const Measurements& m)
    {
        for (auto& [name, id] : m.names) {
            const auto& s = m.stats[id];
            os << name << ": ";
            if (s.count() > 0) {
                os << s.mean();
                if (s.count() > 1)
                    os << " ± " << s.half_ci95() << "";
            }
            os << " μs (" << s.count() << ")\n";
        }
        return os;
    }
};

/// Call this function to consume the data produced by benchmark
/// (otherwise the compiler may optimize away and remove benchmark as dead code)
template <typename T>
void doNotOptimize(T const& value) {
#if defined(__clang__) || defined(__GNUC__)
	asm volatile("" : : "g"(value) : "memory");
#elif defined(_MSC_VER)
    _ReadWriteBarrier();
    (void) value;
#endif
}

#endif // INCLUDE_MEASURE_HPP
