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

template <typename T>
void print_data(std::ostream& os, T&& data)
{
    using value_type = typename std::decay_t<T>::value_type;
    std::copy(data.begin(), data.end(), std::ostream_iterator<value_type>(os, " "));
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
    std::vector<std::string> res;
    std::ranges::transform(data, std::back_inserter(res),
                          [](int d) { return std::to_string(d); });
    return res;
}

std::vector<std::string> compute_sorted(const std::vector<std::string>& data)
{

    auto res = data;  // Make a copy first
    std::sort(res.begin(), res.end(), [](std::string d, std::string s) { return d > s; });
    return res;
}

std::vector<int> squares(const std::vector<int>& data)
{
    auto res = std::vector<int>{};
    std::ranges::transform(data, std::back_inserter(res), [](int d) { return d * d;});
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

void test_stringify(const std::vector<int>& data)
{
    auto res = std::vector<std::string>{};
    for (auto&& d : data)
        res.push_back(std::to_string(d));
    CHECK(res == stringify(data));
}

void test_compute_sorted(const std::vector<std::string>& data)
{
    auto res = compute_sorted(data);

    // 1. Check size hasn't changed
    CHECK(res.size() == data.size());

    // 2. Verify it's actually sorted in decreasing order
    for (size_t i = 1; i < res.size(); ++i) {
        CHECK(res[i-1] >= res[i]);  // Each element >= next (decreasing)
    }

    // 3. Verify all original elements are still present (it's a permutation)
    auto sorted_copy = data;
    std::sort(sorted_copy.begin(), sorted_copy.end());
    auto sorted_res = res;
    std::sort(sorted_res.begin(), sorted_res.end());
    CHECK(sorted_copy == sorted_res);
}

void test_squares(const std::vector<int>& data)
{
    auto res = std::vector<int>{};
    auto compute = squares(data);

    for (auto&& d : data) {
        res.push_back(d * d);
    }
    CHECK(res == compute);
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

    /** Test for stringify */
    auto data_str = stringify(data);
    test_stringify(data);


    /** Test for compute_sorted */
    auto sorted_str = compute_sorted(data_str);
    test_compute_sorted(data_str);


    /** Test for squares */
    test_squares(data);

    // make print_data a function template, so that the following is accepted:
    print_data(std::cout, sorted_str);
    print_data(std::cout, squares(data));
}
