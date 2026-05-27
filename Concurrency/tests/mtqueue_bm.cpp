/**
   Author: Marius Mikucionis <marius@cs.aau.dk>
 */

#include "mtqueue.hpp"

#include <benchmark/benchmark.h>

static void mtqueue_bm(benchmark::State& state)
{
    const auto size = state.range(0);
    for (auto _ : state) {
        auto q = mtqueue<int>{};
        for (auto i = 0; i < size; ++i)
            q.put(i);
        auto sum = 0.;
        for (auto val = q.get(); val; val = q.get())
            sum += val;
        // Because the sum is never printed to the screen,
        // the computer will normally delete the entire removing loop to save time
        benchmark::DoNotOptimize(sum);
        // This command forces the computer to write the results to the computer's memory after the loop
        benchmark::ClobberMemory();
    }
}
// It tells the tool to test the container with 100 items.
// Then, it multiplies the number of items by 10 and tests the container with 1,000 items.
// It repeats this pattern until it reaches a final test of 10,000,000 items.
BENCHMARK(mtqueue_bm)->RangeMultiplier(10)->Range(100, 10'000'000);
