// usr/bin/c++ -std=c++23 -Wpedantic -Wall -Wextra -O3 -DNDEBUG -o fischer fischer.cpp && exec ./fischer
/// Fischer's mutual exclusion protocol based on UPPAAL example
#include <iostream>
#include <chrono>

struct CSMonitor
{
    void enter()
    {
        // TODO: count and check the number of threads, throw if more than one is counted
    }
    void leave()
    {
        // TODO: count the the remaining threads
    }

private:
    // TODO: use data structure to count threads in thread-safe way, also try to avoid memory access interference
};

using namespace std::chrono_literals;  ///< for constants like 100us
using Clock = std::chrono::high_resolution_clock;
using Duration = Clock::duration;

constexpr auto ROUNDS = 1000;  ///< the number of rounds each thread should perform before exiting

using ID = int;  // FIXME: type for shared process identifier, also try to avoid memory access interference

/// Function to be executed by all the threads
void proc(const int pid)  // TODO: arrange sharing of the shared resources
try {
    for (auto round = 0; round < ROUNDS; ++round) {
        // TODO: implement the states:
        // Idle
        // Request
        // Wait
        // CS
        // cs_monitor.enter();
        // store/count the rounds this thread makes for diagnostic purposes
        // cs_monitor.leave();
        // TODO: leave the critical section
    }
} catch (const std::exception& e) {
    std::cerr << "p" << pid << ": " << e.what() << "\n";  // exit the function if an error happens
}

// TODO: create a variable to share the process id, also try to minimize memory access interference

int main()
{
    // TODO: compute the load: the number of threads 4 times larger than the number of cores
    // std::cout << "Starting " << THREADS << " threads..." << std::endl;
    // TODO: launch THREADS number of threads
    std::cout << "Waiting for threads to complete..." << std::endl;
    // TODO: wait for all threads to complete
    // TODO: perform some diagnostics if all threads successfully performed the required number of rounds.
}
