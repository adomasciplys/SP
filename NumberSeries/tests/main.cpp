/// Created by Marius Mikucionis <marius@cs.aau.dk>
/**
 * Purpose: compare the performance of number_series and number_series_wrap.
 */
#include "number_series.hpp"

#include <algorithm>  // sort
#include <chrono>

constexpr auto ns_number = 100'000;
constexpr auto ns_length = 100;

int main()
{
    using namespace DataSeries;
    using Clock = std::chrono::high_resolution_clock;
    using millis = std::chrono::duration<double, std::milli>;  // milli-second units
    using std::cout;
    using std::endl;

    // Part 2
    auto vv = std::vector<NumberSeries>{};
    vv.reserve(ns_number);  // preallocate memory

    // TODO: Populate the vv with data here, like:
    // vv.push_back(NumberSeries::make_random(100); for ns_number of times

    auto t0 = Clock::now();
    // TODO: uncomment next line when NumberSeries is implemented
    // std::ranges::sort(vv, std::less{});
    auto t1 = Clock::now();
    cout << "Sorting values: " << millis(t1 - t0).count() << " ms\n";

    // Part 3
    // Note that this is the exact same code as for Part 1 except using NumberSeriesWrap
    auto vw = std::vector<NumberSeriesWrap>{};
    vw.reserve(ns_number);  // preallocate memory

    // TODO: Populate the vw with data here, like:
    // vw.push_back(NumberSeriesWrap::make_random(100); for ns_number of times

    t0 = Clock::now();
    // TODO: uncomment the next line when NumberSeriesWrap is implemented
    // std::ranges::sort(vw, std::less{});
    t1 = Clock::now();
    cout << "Sorting wrapped pointers: " << millis(t1 - t0).count() << " ms\n";

    cout << "sizeof(NumberSeries): " << sizeof(NumberSeries) << '\n';
}

/**
Important: measure the optimized ("Release") build!

Sample result:

Sorting values: XXX ms
Sorting wrapped pointers: YYY ms
sizeof(NumberSeries): ZZZ

Interpretation:

Sorting NumberSeriesWrap is about XX times ****er than NumberSeries
PUT YOUR CONCLUSION HERE


Sample result, if NumberSeries is padded with a dummy array of 1000 integers:

Sorting values: XXX ms
Sorting wrapped pointers: YYY ms
sizeof(NumberSeries): ZZZ

Sorting NumberSeriesWrap is about YY times ****er than NumberSeries
PUT YOUR CONCLUSION HERE

*/
