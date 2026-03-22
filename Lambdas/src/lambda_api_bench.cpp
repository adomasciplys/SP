/**
 * Benchmarks for lambda_api storage access methods.
 * Compares get_values, apply/for_each, accumulate, and combine
 * across different storage sizes to show the performance impact
 * of type-erasure (std::function) vs. template-based approaches.
 */
#include <benchmark/benchmark.h>
#include "storage.h"

// ── Printer benchmarks ────────────────────────────────────────────────────────
// Each benchmark receives the storage size via state.range(0).
// benchmark::DoNotOptimize() prevents the compiler from optimizing away
// results it thinks are unused — essential for meaningful timings.

// Method 1: get_values() copies the whole collection, then iterates the copy.
static void BM_printer_get_values(benchmark::State& state)
{
    auto d = storage<int>(state.range(0));
    for (auto _ : state) {
        auto sum = 0;
        for (auto&& e : d.get_values())
            benchmark::DoNotOptimize(sum += e);
    }
}

// Method 2: apply() — zero-cost template, lambda inlined at compile time.
static void BM_printer_apply(benchmark::State& state)
{
    auto d = storage<int>(state.range(0));
    for (auto _ : state) {
        auto sum = 0;
        d.apply([&sum](auto e) { benchmark::DoNotOptimize(sum += e); });
    }
}

// Method 3: for_each() — std::function wrapper, virtual-call-like overhead per element.
static void BM_printer_for_each(benchmark::State& state)
{
    auto d = storage<int>(state.range(0));
    for (auto _ : state) {
        auto sum = 0;
        d.for_each([&sum](auto e) { benchmark::DoNotOptimize(sum += e); });
    }
}

// ── Adder benchmarks ──────────────────────────────────────────────────────────

// Method 1: get_values() copy + manual loop sum.
static void BM_adder_get_values(benchmark::State& state)
{
    auto d = storage<double>(state.range(0));
    for (auto _ : state) {
        auto sum = 0.0;
        for (auto&& e : d.get_values())
            sum += e;
        benchmark::DoNotOptimize(sum);
    }
}

// Method 2: for_each() with a capturing lambda — std::function overhead.
static void BM_adder_for_each(benchmark::State& state)
{
    auto d = storage<double>(state.range(0));
    for (auto _ : state) {
        auto sum = 0.0;
        d.for_each([&sum](auto e) { sum += e; });
        benchmark::DoNotOptimize(sum);
    }
}

// Method 3: accumulate() — template fold, lambda inlined, no overhead.
static void BM_adder_accumulate(benchmark::State& state)
{
    auto d = storage<double>(state.range(0));
    auto adder = [](auto a, auto b) { return a + b; };
    for (auto _ : state) {
        auto sum = d.accumulate(0.0, adder);
        benchmark::DoNotOptimize(sum);
    }
}

// Method 4: combine() — same fold but via std::function, type-erasure overhead.
static void BM_adder_combine(benchmark::State& state)
{
    auto d = storage<double>(state.range(0));
    auto adder = [](auto a, auto b) { return a + b; };
    for (auto _ : state) {
        auto sum = d.combine(0.0, adder);
        benchmark::DoNotOptimize(sum);
    }
}

// Register all benchmarks and vary storage size: 100, 1000, 10000, 100000 elements.
// RangeMultiplier(10)->Range(100, 100'000) generates: 100, 1000, 10000, 100000.
BENCHMARK(BM_printer_get_values)->RangeMultiplier(10)->Range(100, 100'000);
BENCHMARK(BM_printer_apply)     ->RangeMultiplier(10)->Range(100, 100'000);
BENCHMARK(BM_printer_for_each)  ->RangeMultiplier(10)->Range(100, 100'000);

BENCHMARK(BM_adder_get_values)  ->RangeMultiplier(10)->Range(100, 100'000);
BENCHMARK(BM_adder_for_each)    ->RangeMultiplier(10)->Range(100, 100'000);
BENCHMARK(BM_adder_accumulate)  ->RangeMultiplier(10)->Range(100, 100'000);
BENCHMARK(BM_adder_combine)     ->RangeMultiplier(10)->Range(100, 100'000);

// benchmark::benchmark_main is provided by benchmark::benchmark_main target in CMake,
// so no manual main() is needed here.
BENCHMARK_MAIN();

/*-----------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations
-----------------------------------------------------------------------
BM_printer_get_values/100          1984 ns         1973 ns       359284
BM_printer_get_values/1000        18570 ns        18513 ns        37989
BM_printer_get_values/10000      191633 ns       188995 ns         3791
BM_printer_get_values/100000    1834995 ns      1832626 ns          374

BM_printer_apply/100               1489 ns         1486 ns       469244 <-- Fastest
BM_printer_apply/1000             14712 ns        14695 ns        47695 <-- Fastest
BM_printer_apply/10000           146997 ns       146792 ns         4766 <-- Fastest
BM_printer_apply/100000         1472068 ns      1469949 ns          469 <-- Fastest

BM_printer_for_each/100            2397 ns         2395 ns       291933
BM_printer_for_each/1000          23752 ns        23717 ns        29503
BM_printer_for_each/10000        235120 ns       234995 ns         2973
BM_printer_for_each/100000      2352727 ns      2350212 ns          297


BM_adder_get_values/100            2115 ns         2114 ns       331188
BM_adder_get_values/1000          20382 ns        20354 ns        34415
BM_adder_get_values/10000        203613 ns       203509 ns         3438
BM_adder_get_values/100000      2037898 ns      2035154 ns          344

BM_adder_for_each/100              2218 ns         2216 ns       314173
BM_adder_for_each/1000            22017 ns        21980 ns        32050
BM_adder_for_each/10000          217297 ns       217149 ns         3217
BM_adder_for_each/100000        2176525 ns      2173655 ns          322

BM_adder_accumulate/100            1592 ns         1470 ns       483559 <-- Fastest
BM_adder_accumulate/1000          14422 ns        14396 ns        48783 <-- Fastest
BM_adder_accumulate/10000        143636 ns       143489 ns         4888 <-- Fastest
BM_adder_accumulate/100000      1444148 ns      1441465 ns          488 <-- Fastest

BM_adder_combine/100               2466 ns         2464 ns       284569
BM_adder_combine/1000             24417 ns        24381 ns        28720
BM_adder_combine/10000           242025 ns       241861 ns         2888
BM_adder_combine/100000         2415941 ns      2414164 ns          287*/
