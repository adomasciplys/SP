/// Tests for measure.hpp
/// Author: Marius Mikucionis marius@cs.aau.dk

/// Keep the assertions enabled even in Release builds:
#ifdef NDEBUG
#undef NDEBUG
#endif // NDEBUG

#include "measure.hpp"

#include <utility>
#include <charconv> // from_chars

#include <cassert>

static_assert(Statistics{1}.mean()==1);
static_assert(Statistics{1,1}.mean()==1);
static_assert(Statistics{1,1,1}.mean()==1);
static_assert(Statistics{1,2,3}.mean()==2);
static_assert(Statistics{1,2,2,2,3}.mean()==2);
void statistics_test()
{
    bool res = (Statistics{1,2,3}.sd()==1);
    assert(res);
    res = (Statistics{1,2,3}.half_ci95()==2.4841377117194634);
    assert(res);
    res = (Statistics{1,2,3}.half_ci99()==5.7301108937148335);
    assert(res);
    res = (Statistics{1,2,2,2,3}.half_ci95()==0.87798903308514809);
    assert(res);
    res = (Statistics{1,2,2,2,3}.half_ci99()==1.4559426357174623);
    assert(res);
}

/// Very inefficient implementation of Fibonacci sequence
constexpr std::size_t fib(std::size_t n) {
	if (n < 2)
		return n;
	return fib(n-1) + fib(n-2);
}
/// compile-time tests for fib
static_assert(fib(0)==0);
static_assert(fib(1)==1);
static_assert(fib(2)==1);
static_assert(fib(3)==2);
static_assert(fib(4)==3);
static_assert(fib(5)==5);
static_assert(fib(6)==8);
static_assert(fib(7)==13);

/// Faster implementation of Fibonacci sequence
constexpr std::size_t fibonacci(std::size_t n) {
	if (n < 2)
		return n;
	auto a = 0;
	auto b = 1;
	--n;
	while (n-->0)
		a = std::exchange(b, a+b);
	return b;
}
/// compile-time tests for fibonacci
static_assert(fibonacci(0)==0);
static_assert(fibonacci(1)==1);
static_assert(fibonacci(2)==1);
static_assert(fibonacci(3)==2);
static_assert(fibonacci(4)==3);
static_assert(fibonacci(5)==5);
static_assert(fibonacci(6)==8);
static_assert(fibonacci(7)==13);

/// Storage for benchmark measurements
auto meas = Measurements{};

/// Benchmark for fib
void fib_bm(std::size_t n, std::size_t count = 100) {
	const auto sample = meas.create(); // create measurement sample
	for (auto i = 0uz; i < count; ++i) { // repeat for `count` times
		meas.start(sample); // start the measurement for the sample
		const auto res = fib(n); // execute the function to be measured
		doNotOptimize(res); // tell compiler that `res` is used (otherwise it will be optimized away)
		meas.stop(sample); // stop and record the measurement, add it to the sample
	}
}

/// Benchmark for fib
void fib_bm2(std::size_t n, std::size_t count = 100) {
	for (auto i = 0uz; i < count; ++i) {
		auto sw = meas.launch(); // create a stopwatch to measure the duration of this scope
		const auto res = fib(n); // execute the function to be measured
		doNotOptimize(res); // tell compiler to not remove the code resulting in `res` that we don't use.
	}
}

/// Benchmark for fibonacci
void fibonacci_bm(std::size_t n, std::size_t count = 100) {
	const auto id = meas.create();
	for (auto i = 0uz; i < count; ++i) {
		meas.start(id);
		const auto res = fibonacci(n);
		doNotOptimize(res);
		meas.stop(id);
	}
}

/// Benchmark for fibonacci
void fibonacci_bm2(std::size_t n, std::size_t count = 100) {
	for (auto i = 0uz; i < count; ++i) {
		auto sw = meas.launch();
		const auto res = fibonacci(n);
		doNotOptimize(res);
	}
}

int main(int argc, const char* args[])
{
    statistics_test();
	auto n = 20uz;
	if (argc < 2) {
		std::cerr << "Specify a positive number as argument\n";
		//return 1;
	} else if (auto [p,ec] = std::from_chars(args[1], nullptr, n); ec != std::errc()) {
		std::cerr << "Failed to parse as a number: " << args[1] << '\n';
		return 1;
	}
	fib_bm(n);
	fib_bm2(n);
	fibonacci_bm(n);
	fibonacci_bm2(n);
	std::cout << meas;
}
