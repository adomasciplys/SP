#include "vessel.hpp"

#include <doctest/doctest.h>

#include <stdexcept>

using stochastic::Vessel;

TEST_CASE("Vessel::add(species): duplicates throw")
{
    Vessel v{"v"};
    v.add("A", 1);
    CHECK_THROWS_AS(v.add("A", 999), std::invalid_argument);
    CHECK(v.species().size() == 1);
    CHECK(v.species()[v.find_index("A")].initial_count == 1);
}

TEST_CASE("Vessel::find_index: missing species throws")
{
    Vessel v{"v"};
    v.add("A", 1);
    CHECK_THROWS_AS(static_cast<void>(v.find_index("B")), std::out_of_range);
}

TEST_CASE("Vessel::environment: marker is the empty name reactant")
{
    Vessel v{"v"};
    const auto env = v.environment();
    CHECK(env.is_environment());
    CHECK(env.name.empty());
}

TEST_CASE("DSL: (A + B) >> rate >>= C + D builds a Reaction with the right shape")
{
    Vessel v{"v"};
    const auto A = v.add("A", 1);
    const auto B = v.add("B", 1);
    const auto C = v.add("C", 0);
    const auto D = v.add("D", 0);

    v.add((A + B) >> 0.25 >>= C + D);

    REQUIRE(v.reactions().size() == 1);
    const auto& r = v.reactions().front();
    CHECK(r.rate == doctest::Approx(0.25));
    REQUIRE(r.inputs.reactants.size() == 2);
    CHECK(r.inputs.reactants[0].name == "A");
    CHECK(r.inputs.reactants[1].name == "B");
    REQUIRE(r.products.reactants.size() == 2);
    CHECK(r.products.reactants[0].name == "C");
    CHECK(r.products.reactants[1].name == "D");
}

TEST_CASE("DSL: decay reaction A >> rate >>= env")
{
    Vessel v{"v"};
    const auto env = v.environment();
    const auto A = v.add("A", 1);

    v.add(A >> 0.1 >>= env);

    REQUIRE(v.reactions().size() == 1);
    const auto& r = v.reactions().front();
    REQUIRE(r.products.reactants.size() == 1);
    CHECK(r.products.reactants[0].is_environment());
}
