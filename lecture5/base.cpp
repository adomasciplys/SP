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


