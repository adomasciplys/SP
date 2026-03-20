/**
 * Purpose: exercise lambda expressions with STL algorithms.
 * @author Marius Mikucionis <marius@cs.aau.dk>
 */
#include <algorithm>
#include <doctest/doctest.h>
#include <iostream>
#include <iterator>
#include <vector>
#include <random>
#include <sstream>

static auto gen = std::mt19937{std::random_device{}()};
static auto dist = std::uniform_int_distribution<int>{0, 100};

void fill_with_data(std::vector<int>& data, size_t count)
{
    std::generate_n(std::back_inserter(data), count, [](){ return dist(gen); });
}

void print_data(std::ostream& os, const std::vector<int>& data)
{
    std::copy(data.begin(), data.end(), std::ostream_iterator<int>(os, " "));
    os << '\n';
}

int compute_sum(const std::vector<int>& data)
{
    return std::ranges::fold_left(data, 0.0, [](int s, const int it){ return s+it; });
}

std::string concatenate(const std::vector<int>& data)
{
    return std::ranges::fold_left(data, " ", [](std::string s, const int it){ return s + std::to_string(it); });
}

size_t odd_count(const std::vector<int>& data)
{
    return std::count_if(data.begin(), data.end(), [](auto d) { return d % 2 == 1; });
}

std::vector<std::string> stringify(const std::vector<int>& data)
{
    /** TODO: use transform instead */
    auto res = std::vector<std::string>{};
    for (auto&& d : data)
        res.push_back(std::to_string(d));
    return res;
}

std::vector<std::string> compute_sorted(const std::vector<std::string>& data)
{
    /** TODO: use std::sort */
    auto res = data;
    // sorting implementation of decreasing alphabetical order
    return res;
}

std::vector<int> squares(const std::vector<int>&)
{
    /** TODO: find and use an applicable std algorithm */
    auto res = std::vector<int>{};
    // compute square of each input element
    return res;
}

void test_print_data(const std::vector<int>& data)
{
    auto os = std::ostringstream{};
    print_data(os, data);

    // Build expected output
    auto expected = std::ostringstream{};
    for (auto d : data) {
        expected << d << " ";
    }
    expected << '\n';

    // Verify they match
    CHECK(os.str() == expected.str());
}

void test_concatenate(const std::vector<int>& data)
{
    auto result = concatenate(data);

    // Build expected output: start with " ", then append each number as string
    auto expected = std::string{" "};
    for (auto d : data) {
        expected += std::to_string(d);
    }

    CHECK(result == expected);
}

void test_odd_count(const std::vector<int>& data)
{
    auto actual_count = 0u;
    for (auto&& d : data)
        if (d % 2 == 1)
            ++actual_count;
    CHECK(actual_count == odd_count(data));
}


TEST_CASE("Lambda puzzle3")
{
    auto data = std::vector<int>{};
    fill_with_data(data, 20);

    /** Test for print_data */
    test_print_data(data);
    print_data(std::cout, data);

    /** Test for compute_sum */
    int sum = std::accumulate(data.begin(), data.end(), 0);
    CHECK(sum == compute_sum(data));
    std::cout << "sum: " << compute_sum(data) << std::endl;

    /** Test for concatenate */
    test_concatenate(data);
    std::cout << "concat: " << concatenate(data) << std::endl;

    /** Test for odd_count */
    test_odd_count(data);
    std::cout << "odd count: " << odd_count(data) << std::endl;

    /** TODO: write test for stringify */
    auto data_str = stringify(data);
    /** TODO: write test for compute_sorted */
    auto sorted_str = compute_sorted(data_str);
    /** TODO: write test for squares */
    // make print_data a function template, so that the following is accepted:
    // print_data(std::cout, sorted_str);
    print_data(std::cout, squares(data));
}
