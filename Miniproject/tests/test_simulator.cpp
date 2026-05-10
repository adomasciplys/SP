#include "simulator.hpp"
#include "vessel.hpp"

#include <doctest/doctest.h>

using stochastic::Simulator;
using stochastic::Vessel;

TEST_CASE("Simulator: counts() seeded from species' initial_count, time() starts at 0")
{
    Vessel v{"v"};
    v.add("A", 5);
    v.add("B", 0);

    Simulator sim{v, 42};

    REQUIRE(sim.counts().size() == 2);
    CHECK(sim.counts()[0] == 5);
    CHECK(sim.counts()[1] == 0);
    CHECK(sim.time() == doctest::Approx(0.0));
}

TEST_CASE("Simulator: A >>= B advances time and moves one molecule on step()")
{
    Vessel v{"v"};
    const auto A = v.add("A", 1);
    const auto B = v.add("B", 0);
    v.add(A >> 1.0 >>= B);

    Simulator sim{v, 42};
    sim.step();

    CHECK(sim.time() > 0.0);
    CHECK(sim.counts()[0] == 0);  // A consumed
    CHECK(sim.counts()[1] == 1);  // B produced
}

TEST_CASE("Simulator: mass conservation in A >>= B")
{
    Vessel v{"v"};
    const auto A = v.add("A", 100);
    const auto B = v.add("B", 0);
    v.add(A >> 1.0 >>= B);

    Simulator sim{v, 42};
    sim.simulate(1000.0);

    CHECK(sim.counts()[0] + sim.counts()[1] == 100);
}

TEST_CASE("Simulator: decay A >>= env drains A to zero")
{
    Vessel v{"v"};
    const auto env = v.environment();
    const auto A = v.add("A", 10);
    v.add(A >> 1.0 >>= env);

    Simulator sim{v, 42};
    sim.simulate(1000.0);

    CHECK(sim.counts()[0] == 0);
}

TEST_CASE("Simulator: creation env >>= A grows A from zero")
{
    Vessel v{"v"};
    const auto env = v.environment();
    const auto A = v.add("A", 0);
    v.add(env >> 1.0 >>= A);

    Simulator sim{v, 42};
    sim.simulate(10.0);

    CHECK(sim.counts()[0] > 0);
}

TEST_CASE("Simulator: catalyst (S+I) >>= (E+I) preserves the catalyst count")
{
    Vessel v{"v"};
    const auto S = v.add("S", 100);
    const auto E = v.add("E", 0);
    const auto I = v.add("I", 5);
    v.add((S + I) >> 0.01 >>= E + I);

    Simulator sim{v, 42};
    sim.simulate(50.0);

    CHECK(sim.counts()[2] == 5);                       // I unchanged
    CHECK(sim.counts()[0] + sim.counts()[1] == 100);   // S + E conserved
}

TEST_CASE("Simulator: same seed produces an identical trajectory")
{
    Vessel v{"v"};
    const auto A = v.add("A", 50);
    const auto B = v.add("B", 0);
    v.add(A >> 1.0 >>= B);

    Simulator sim1{v, 12345};
    Simulator sim2{v, 12345};
    sim1.simulate(5.0);
    sim2.simulate(5.0);

    CHECK(sim1.time() == doctest::Approx(sim2.time()));
    CHECK(sim1.counts() == sim2.counts());
}

TEST_CASE("Simulator: 2A >>= B does not fire when only one A is present")
{
    Vessel v{"v"};
    const auto A = v.add("A", 1);
    const auto B = v.add("B", 0);
    v.add((A + A) >> 1.0 >>= B);  // multiplicity: needs two A's

    Simulator sim{v, 42};
    sim.step();

    // A non-zero count of A gives a finite delay (input_product = 1), so time
    // advances; can_fire then rejects the firing because state[A] < 2.
    CHECK(sim.time() > 0.0);
    CHECK(sim.counts()[0] == 1);
    CHECK(sim.counts()[1] == 0);
}

TEST_CASE("Simulator: 2A >>= B fires when two A's are present")
{
    Vessel v{"v"};
    const auto A = v.add("A", 2);
    const auto B = v.add("B", 0);
    v.add((A + A) >> 1.0 >>= B);

    Simulator sim{v, 42};
    sim.step();

    CHECK(sim.counts()[0] == 0);
    CHECK(sim.counts()[1] == 1);
}

TEST_CASE("Simulator: simulate(end_time) terminates with time() >= end_time")
{
    Vessel v{"v"};
    const auto A = v.add("A", 100);
    const auto B = v.add("B", 0);
    v.add(A >> 0.01 >>= B);

    Simulator sim{v, 42};
    sim.simulate(10.0);

    CHECK(sim.time() >= 10.0);
}

TEST_CASE("Simulator: run() yields the initial state, then one Sample per step")
{
    Vessel v{"v"};
    const auto A = v.add("A", 3);
    const auto B = v.add("B", 0);
    v.add(A >> 1.0 >>= B);

    Simulator sim{v, 42};

    std::vector<Simulator::Sample> samples;
    for (const auto& s : sim.run(100.0))
        samples.push_back(s);

    // Initial sample + one per A-to-B firing (3 firings) = 4 samples.
    // After A is depleted, the trajectory ends (no more events possible).
    REQUIRE(samples.size() == 4);

    CHECK(samples[0].time == doctest::Approx(0.0));
    CHECK(samples[0].counts[0] == 3);
    CHECK(samples[0].counts[1] == 0);

    // Time strictly increases.
    for (std::size_t i = 1; i < samples.size(); ++i)
        CHECK(samples[i].time > samples[i - 1].time);

    // Final sample: all A consumed, all became B.
    CHECK(samples.back().counts[0] == 0);
    CHECK(samples.back().counts[1] == 3);
}

TEST_CASE("Simulator: run() can be consumed without storing the whole trajectory")
{
    // The exact use-case the assignment calls out: track a single statistic
    // (here, the maximum count of B) without holding the trajectory in memory.
    Vessel v{"v"};
    const auto A = v.add("A", 50);
    const auto B = v.add("B", 0);
    v.add(A >> 1.0 >>= B);

    Simulator sim{v, 42};

    std::size_t peak_B = 0;
    for (const auto& s : sim.run(1000.0))
        peak_B = std::max(peak_B, s.counts[1]);

    CHECK(peak_B == 50);  // mass conservation: peak B equals initial A
}

TEST_CASE("Simulator: depleted input stops one reaction but not others")
{
    Vessel v{"v"};
    const auto A = v.add("A", 1);   // depletes immediately
    const auto B = v.add("B", 0);
    const auto C = v.add("C", 5);
    const auto D = v.add("D", 0);
    v.add(A >> 100.0 >>= B);        // fast, depletes A
    v.add(C >> 0.5 >>= D);

    Simulator sim{v, 42};
    sim.simulate(50.0);

    CHECK(sim.counts()[0] == 0);                       // A gone
    CHECK(sim.counts()[1] == 1);                       // all became B
    CHECK(sim.counts()[2] + sim.counts()[3] == 5);     // C + D conserved
}
