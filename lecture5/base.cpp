#include <benchmark/benchmark.h>
#include <vector>
#include <memory>

struct Base {
    int x = 0;
    virtual void increment() { ++x; }
    void decrement() { --x; }
};

struct Derived final : Base {
    void increment() override { ++x; }
};

static void BM_Increment(benchmark::State& state) {
    std::vector<std::unique_ptr<Base>> vec;
    vec.reserve(10'000'000);
    for (int i = 0; i < 10'000'000; ++i) {
        vec.push_back(std::make_unique<Derived>());
    }

    for (auto _ : state) {
        for (auto& ptr : vec) {
            ptr->increment();
        }
    }
}

static void BM_Decrement(benchmark::State& state) {
    std::vector<std::unique_ptr<Base>> vec;
    vec.reserve(10'000'000);
    for (int i = 0; i < 10'000'000; ++i) {
        vec.push_back(std::make_unique<Derived>());
    }

    for (auto _ : state) {
        for (auto& ptr : vec) {
            ptr->decrement();
        }
    }
}

static void BM_Increment_NoPointers(benchmark::State& state) {
    std::vector<Derived> vec(10'000'000);

    for (auto _ : state) {
        for (auto& obj : vec) {
            obj.increment();
        }
    }
}

static void BM_Decrement_NoPointers(benchmark::State& state) {
    std::vector<Derived> vec(10'000'000);

    for (auto _ : state) {
        for (auto& obj : vec) {
            obj.decrement();
        }
    }
}

BENCHMARK(BM_Increment);
BENCHMARK(BM_Decrement);
BENCHMARK(BM_Increment_NoPointers);
BENCHMARK(BM_Decrement_NoPointers);


BENCHMARK_MAIN();

// --- WITHOUT FINAL --

// ------------------------------------------------------------------
// Benchmark                        Time             CPU   Iterations
// ------------------------------------------------------------------
// BM_Increment               9573678 ns      9567845 ns           71
// BM_Decrement               4855230 ns      4843770 ns          148
// BM_Increment_NoPointers    5117911 ns      5115810 ns          137
// BM_Decrement_NoPointers    1897239 ns      1888893 ns          373

// --- WITH FINAL --

// ------------------------------------------------------------------
// Benchmark                        Time             CPU   Iterations
// ------------------------------------------------------------------
// BM_Increment               9568255 ns      9562423 ns           71
// BM_Decrement               4828388 ns      4820728 ns          147
// BM_Increment_NoPointers    1858085 ns      1857459 ns          366
// BM_Decrement_NoPointers    1881991 ns      1879146 ns          370


// Virtual dispatch is expensive:
// Calling increment() through a base class pointer (9.5ms) is ~5x slower than calling it on a concrete object (1.8ms).
// This is because virtual calls can't be inlined and require runtime dispatch.

// Non-virtual functions are fast:
// decrement() is consistently fast (~4.8ms with pointers, ~1.8ms without) because it's non-virtual and can be inlined.
// Pointer indirection matters: Even non-virtual calls are slower through pointers (4.8ms)
// than direct access (1.8ms) due to cache misses and dereferencing overhead

// When Derived is final, the compiler knows it can't be subclassed, so it can devirtualize the call to increment()
// even through the base class pointer in the original benchmark.
// Without final, the compiler must assume a subclass could override the method, so it can't optimize away the virtual dispatch.
