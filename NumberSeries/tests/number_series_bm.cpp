/// Created by Marius Mikucionis <marius@cs.aau.dk>
#include "number_series.hpp"
#include <benchmark/benchmark.h>

auto make_vv(size_t ns_number, size_t ns_length)
{
    using DataSeries::NumberSeries;
    auto vv = std::vector<NumberSeries>{};
    vv.reserve(ns_number);  // preallocate memory

    for (size_t i = 0; i < ns_number; ++i) {
        vv.emplace_back(NumberSeries::make_random(100));
    }

    return vv;
};

auto make_wrapper(size_t ns_number, size_t ns_length)
{
    using DataSeries::NumberSeriesWrap;
    auto vw = std::vector<NumberSeriesWrap>{};
    vw.reserve(ns_number);  // preallocate memory

    for (size_t i = 0; i < ns_number; ++i) {
        vw.emplace_back(NumberSeriesWrap::make_random(100));
    }

    return vw;
};

static void ns_sort(benchmark::State& state)
{
    const auto ns_number = state.range(0);
    const auto ns_length = state.range(1);
    const auto input = make_vv(ns_number, ns_length);
    for (auto _ : state) {
        state.PauseTiming();
        auto vv = input;
        state.ResumeTiming();
        std::sort(vv.begin(), vv.end());
        benchmark::DoNotOptimize(vv.data());  // tells compiler that vv.data() is useful
        benchmark::ClobberMemory();           // flush changes to memory
    }
}
BENCHMARK(ns_sort)->ArgPair(100'000, 100)->ArgPair(10'000, 100)->ArgPair(1'000, 100);

// ------------------- BEFORE ADDING DUMMY ----------------------------------------------
// Benchmark                       Time             CPU   Iterations
// -----------------------------------------------------------------
// ns_sort/100000/100        7914085 ns      7899182 ns           88
// ns_sort/10000/100          378186 ns       376523 ns         1836
// ns_sort/1000/100            25858 ns        25778 ns        26241
// nswrap_sort/100000/100   10903951 ns     10881371 ns           62
// nswrap_sort/10000/100      852711 ns       851663 ns         1053
// nswrap_sort/1000/100        45533 ns        45489 ns        16014

// -----------------------------------------------------------------
//   Benchmark                       Time             CPU   Iterations
// -----------------------------------------------------------------
// ns_sort/100000/100       26684981 ns     26443692 ns           26
// ns_sort/10000/100         1827401 ns      1818430 ns          393
// ns_sort/1000/100           115806 ns       114600 ns         6005
// nswrap_sort/100000/100   19260703 ns     18899162 ns           37
// nswrap_sort/10000/100     1723111 ns      1684346 ns          593
// nswrap_sort/1000/100        55820 ns        55434 ns        11514

static void nswrap_sort(benchmark::State& state)
{
    const auto ns_number = state.range(0);
    const auto ns_length = state.range(1);
    const auto input = make_wrapper(ns_number, ns_length);
    for (auto _ : state) {
        state.PauseTiming();
        auto vw = input;
        state.ResumeTiming();
        std::sort(vw.begin(), vw.end());
        benchmark::DoNotOptimize(vw.data());  // tells compiler that vw.data() is useful
        benchmark::ClobberMemory();           // flush changes to memory
    }
}
BENCHMARK(nswrap_sort)->ArgPair(100'000, 100)->ArgPair(10'000, 100)->ArgPair(1'000, 100);


