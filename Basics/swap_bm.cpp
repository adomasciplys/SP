#include <random>
#include <iostream>
#include "passing.hpp"
#include "measure.hpp"
#include "swap.hpp"


Measurements meas;
constexpr unsigned COUNT = 1000;

void swap_ends_array_bm(int input[], std::size_t size, const std::size_t count = COUNT)
{
    const auto id = meas.create("swap_ends_array_bm");
    for (std::size_t i = 0; i < count; ++i)
    {
        meas.start(id);
        swap_ends_optimal(input, size);
        doNotOptimize(input[0]);
        doNotOptimize(input[size - 1]);
        meas.stop(id);
    }
}

void swap_ends_vector_bm(std::vector<int>& input, const size_t count = COUNT)
{
    const auto id = meas.create("swap_ends_vector_bm");
    for (std::size_t i = 0; i < count; ++i)
    {
        meas.start(id);
        swap_ends(input);
        doNotOptimize(input[0]);
        doNotOptimize(input.back());
        meas.stop(id);
    }
}

int main()
{
    constexpr unsigned SIZE = 1000;
    int ai[SIZE];
    std::mt19937 gen{std::random_device{}()}; // random number generator
    std::uniform_int_distribution<int> dist{0, 10}; // distribution
    for (unsigned i = 0; i < SIZE; ++i)
        ai[i] = dist(gen); // generates random numbers 0..10
    std::vector vi(ai, ai + SIZE); // creates a copy
    swap_ends_array_bm(ai, SIZE);
    swap_ends_vector_bm(vi, SIZE);
    std::cout << meas;
}
