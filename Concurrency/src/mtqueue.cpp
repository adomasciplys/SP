#include "mtqueue.hpp"

#include <iostream>
#include <random>
#include <thread>

void produce(mtqueue<int>& queue, size_t count)
{
    // Thread-local RNG avoids sharing one engine across threads.
    thread_local std::default_random_engine gen{std::random_device{}()};
    std::uniform_int_distribution<int> dist(0, 1000000);

    for (size_t i = 0; i < count; ++i) {
        queue.put(dist(gen));
    }
}

void consume(mtqueue<int>& queue, size_t count)
{
    size_t consumed = 0;
    while (consumed < count) {
        if (auto value = queue.get()) {
            std::cout << *value << '\n';
            ++consumed;
        } else {
            // let other threads run.
            std::this_thread::yield();
        }
    }
}

int main()
{
    mtqueue<int> queue;

    constexpr size_t producer_count = 2;
    constexpr size_t consumer_count = 2;
    constexpr size_t items_per_producer = 1000;
    constexpr size_t total_items = producer_count * items_per_producer;
    constexpr size_t items_per_consumer = total_items / consumer_count;

    std::vector<std::jthread> threads;
    threads.reserve(producer_count + consumer_count);

    // Producers
    for (size_t i = 0; i < producer_count; ++i) {
        threads.emplace_back(produce, std::ref(queue), items_per_producer);
    }

    // Consumers
    for (size_t i = 0; i < consumer_count; ++i) {
        threads.emplace_back(consume, std::ref(queue), items_per_consumer);
    }

    // No explicit join() needed:
    // each std::jthread joins automatically in its destructor.

    // NOTE: producer and consumer expect to bind to the queue by reference:
    // 1) STL uses values, therefore plain queue argument will not work: wrap the queue into `std::ref` instead.
    // 2) Using `std::ref` is not normal: it is a red-flag for race conditions, which needs to be addressed by design.
    // In this case multiple threads share the queue which is supposed to be safe in multi-threaded (MT) environment.
    // NOTE: many successful test runs do not guarantee that the queue is MT-safe.
    // You may make your producers to work longer, try running optimized builds hoping to trigger an error,
    // or enable option(TSAN "Thread Sanitizer" ON) in the CMakeLists.txt
    // (remember to regenerate the cmake build by deleting the build directories).
    // Limitations:
    //  - clang++-13 does not ship std::jthread.
    //  - thread sanitizer is supported only on Linux and Apple Clang.
}