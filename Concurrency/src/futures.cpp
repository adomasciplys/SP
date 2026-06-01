#include "../../../../../../../../../opt/homebrew/Cellar/gcc/15.2.0/include/c++/15/iostream"

#include <vector>
#include <future>
#include <random>
#include <algorithm>
#include <numeric>
#include <thread>     // for std::jthread
#include <iostream>


std::vector<int> produce(size_t count){
    std::vector<int> res(count);

    // thread-local RNG so each thread gets its own generator
    thread_local std::mt19937_64 gen{std::random_device{}()};
    std::uniform_int_distribution<int> dist(0, 50);

    std::generate(res.begin(), res.end(), [&]{ return dist(gen); });
    return res;

}

int summarize(std::vector<std::future<std::vector<int>>>&& futures){
    int total = 0;

    for (auto &future : futures) {
        auto values = future.get();
        total += std::accumulate(values.begin(), values.end(), 0);
    }

    return total;
}

int main()
{
    const int producer_count = 4;
    const int item_count = 100'000;
    std::vector<std::jthread> threads;
    std::vector<std::future<std::vector<int>>> futures;
    futures.reserve(producer_count);
    threads.reserve(producer_count);

    for (size_t i = 0; i < producer_count; ++i) {
        std::promise<std::vector<int>> p;
        futures.push_back(p.get_future());
        // move the promise into the thread's lambda
        threads.emplace_back([p = std::move(p)](std::stop_token) mutable {
            // produce and set the promise
            p.set_value(produce(item_count));
        });
    }

    int sum = summarize(std::move(futures));
    std::cout << sum << std::endl;
    return 0;
}