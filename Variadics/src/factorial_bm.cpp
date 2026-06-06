#include "factorial.hpp"
#include <benchmark/benchmark.h>

void fact_rt_bm(benchmark::State& state)
{
	auto i = 40u; // runtime variable
	for (auto _ : state){
		auto v = fact(i); // runtime execution
		benchmark::DoNotOptimize(v);
		benchmark::ClobberMemory();
	}
}
BENCHMARK(fact_rt_bm);

void fact_ct_bm(benchmark::State& state)
{
	constexpr auto i = 40u; // compile-time value
	for (auto _ : state){
		auto v = fact(i);
		benchmark::DoNotOptimize(v); // compile-time execution
		benchmark::ClobberMemory();
	}
}
BENCHMARK(fact_ct_bm);

void fact_meta_bm(benchmark::State& state)
{
	constexpr auto i = 40u;
	for (auto _ : state){
		auto v = Fact<i>::value; // requires compile-time value
		benchmark::DoNotOptimize(v);
		benchmark::ClobberMemory();
	}
}
BENCHMARK(fact_meta_bm);


BENCHMARK_MAIN();