/// Benchmarks for the functions in the `passing` library.
/// Author: Marius Mikucionis marius@cs.aau.dk

#include "passing.hpp" // library header with declarations

#include "measure.hpp" //

// Are the benchmark code equivalent and fair for each function?
// Same number of iterations (count) and same measurement boundaries around one call each iteration.
// Same input value (value) and a single arg created outside the timed loop
// So I would say so?

constexpr auto COUNT = 5000uz;
auto meas = Measurements{};

/// Measures pass_by_value(Small)
void pass_by_value_small_bm(int value, std::size_t count = COUNT) {
	const auto sample = meas.create("pass_by_value Small"); // create a named measurement sample
	const auto arg = Small{{value,value}}; // create input
	for (auto i = 0uz; i < count; ++i) { // repeat for `count` times
		meas.start(sample); // start a measurement
		const auto res = pass_by_value(arg); // call the function and collect the result
		doNotOptimize(res); // tell compiler not to remove code resulting in `res` even if we don't use it
		meas.stop(sample); // stop and record the measurement, add it to the sample
	}
}

/// Measures pass_by_ptr(Small)
void pass_by_ptr_small_bm(int value, std::size_t count = COUNT) {
	const auto id = meas.create("pass_by_ptr   Small");
	auto arg = Small{{value,value}};
	for (auto i = 0uz; i < count; ++i) {
		meas.start(id);
		const auto res = pass_by_ptr(&arg);
		doNotOptimize(res);
		meas.stop(id);
	}
}

/// Measures pass_by_ref(Small)
void pass_by_ref_small_bm(int value, std::size_t count = COUNT) {
	const auto id = meas.create("pass_by_ref   Small");
	auto arg = Small{{value,value}};
	for (auto i = 0uz; i < count; ++i) {
		meas.start(id);
		const auto res = pass_by_ref(arg);
		doNotOptimize(res);
		meas.stop(id);
	}
}

/// Measures pass_by_cref(Small)
void pass_by_cref_small_bm(int value, std::size_t count = COUNT) {
	const auto id = meas.create("pass_by_cref  Small");
	const auto arg = Small{{value,value}};
	for (auto i = 0uz; i < count; ++i) {
		meas.start(id);
		const auto res = pass_by_cref(arg);
		doNotOptimize(res);
		meas.stop(id);
	}
}

/// Measures pass_by_value(Large)
void pass_by_value_large_bm(int value, std::size_t count = COUNT) {
	const auto id = meas.create("pass_by_value Large");
	const auto arg = Large{{value,value}};
	for (auto i = 0uz; i < count; ++i) {
		meas.start(id);
		const auto res = pass_by_value(arg);
		doNotOptimize(res);
		meas.stop(id);
	}
}

/// Measures pass_by_ptr(Large)
void pass_by_ptr_large_bm(int value, std::size_t count = COUNT) {
	const auto id = meas.create("pass_by_ptr   Large");
	auto arg = Large{{value,value}};
	for (auto i = 0uz; i < count; ++i) {
		meas.start(id);
		const auto res = pass_by_ptr(&arg);
		doNotOptimize(res);
		meas.stop(id);
	}
}

/// Measures pass_by_ref(Large)
void pass_by_ref_large_bm(int value, std::size_t count = COUNT) {
	const auto id = meas.create("pass_by_ref   Large");
	auto arg = Large{{value,value}};
	for (auto i = 0uz; i < count; ++i) {
		meas.start(id);
		const auto res = pass_by_ref(arg);
		doNotOptimize(res);
		meas.stop(id);
	}
}

/// Measures pass_by_cref(Large)
void pass_by_cref_large_bm(int value, std::size_t count = COUNT) {
	const auto id = meas.create("pass_by_cref  Large");
	const auto arg = Large{{value,value}};
	for (auto i = 0uz; i < count; ++i) {
		meas.start(id);
		const auto res = pass_by_cref(arg);
		doNotOptimize(res);
		meas.stop(id);
	}
}

int main(int argc, const char* args[]) {
	int n = 5; // dummy argument to prepare inputs (ensure that the results are not pre-computed at compile time)
	if (argc < 2) {
		std::cerr << "Specify an integer as argument\n";
		//return 1;
	} else if (auto [p,ec] = std::from_chars(args[1], nullptr, n); ec != std::errc()) {
		std::cerr << "Failed to parse as a number: " << args[1] << '\n';
		// return 1;
	}
	// Measure all functions with Small data:
	pass_by_value_small_bm(n);
	pass_by_ptr_small_bm(n);
	pass_by_ref_small_bm(n);
	pass_by_cref_small_bm(n);

	// Measure all functions with Large data:
	pass_by_value_large_bm(n);
	pass_by_ptr_large_bm(n);
	pass_by_ref_large_bm(n);
	pass_by_cref_large_bm(n);
	// Report all measurements:
	std::cout << meas;
}
