/// Created by Marius Mikucionis <marius@cs.aau.dk>
#include "number_series.hpp"
#include <benchmark/benchmark.h>

auto make_vv(size_t ns_number, size_t ns_length)
{
    using DataSeries::NumberSeries;
    auto vv = std::vector<NumberSeries>{};
    vv.reserve(ns_number);  // preallocate memory

    for (size_t i = 0; i < ns_number; ++i) {
        vv.emplace_back(NumberSeries::make_random(ns_length));
    }

    return vv;
};

auto make_wrapper(size_t ns_number, size_t ns_length)
{
    using DataSeries::NumberSeriesWrap;
    auto vw = std::vector<NumberSeriesWrap>{};
    vw.reserve(ns_number);  // preallocate memory

    for (size_t i = 0; i < ns_number; ++i) {
        vw.emplace_back(NumberSeriesWrap::make_random(ns_length));
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

 // ----------------- BEFORE ADDING DUMMY DATA------------------------------------------------
 // Benchmark                       Time             CPU   Iterations
 // -----------------------------------------------------------------
 // ns_sort/100000/100        7863218 ns      7846057 ns           88
 // ns_sort/10000/100          358389 ns       357360 ns         1936
 // ns_sort/1000/100            26504 ns        26460 ns        27297
 // nswrap_sort/100000/100   12793249 ns     12728849 ns           53
 // nswrap_sort/10000/100      649653 ns       645332 ns         1124
 // nswrap_sort/1000/100        49560 ns        49457 ns        14211

// --------------------- AFTER ADDING DUMMY DATA --------------------------------------------
// Benchmark                       Time             CPU   Iterations
//  -----------------------------------------------------------------
//  ns_sort/100000/100       27963196 ns     26655821 ns           28
//  ns_sort/10000/100         2024977 ns      1729912 ns          410
//  ns_sort/1000/100           109612 ns       108881 ns         6622
//  nswrap_sort/100000/100   20734535 ns     19682143 ns           35
//  nswrap_sort/10000/100      918942 ns       909594 ns          845
//  nswrap_sort/1000/100        50777 ns        50663 ns        13411

//  If I include the dummy data then NumberSeriesWrap sorting becomes faster than NumberSeries sorting
//  Before dummy: NumberSeriesWrap is slower (because NumberSeries objects are small?)
//  After dummy: move/swap cost dominates for plain NumberSeries, and NumberSeriesWrap wins by moving pointers.


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


