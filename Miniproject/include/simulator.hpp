#ifndef MINIPROJECT_SIMULATOR_HPP
#define MINIPROJECT_SIMULATOR_HPP

#include "generator.hpp"
#include "vessel.hpp"

#include <cstddef>
#include <random>
#include <vector>

namespace stochastic
{
    // Stochastic simulator (Algorithm 1).
    // Borrows an immutable Vessel and owns the state + RNG.
    // Result: Independent simulations can run against the same Vessel concurrently.
    struct Simulator
    {
        // One trajectory point: time + a snapshot of all species counts.
        struct Sample
        {
            double time;
            std::vector<std::size_t> counts;
        };

        Simulator(const Vessel& vessel, std::size_t seed);

        // Full trajectory: Run simulation forward until t >= end_time
        void simulate(double end_time);

        // Lazy trajectory: Yields the initial state
        // Then one Sample after every successful step, until t >= end_time.
        Generator<Sample> run(double end_time);

        // One iteration of Algorithm 1
        void step();

        [[nodiscard]] double time() const noexcept { return t; }
        [[nodiscard]] const std::vector<std::size_t>& counts() const noexcept { return state; }

    private:
        // Use Equation 1, to compute the delays for each reaction
        void compute_delays();

        // Helper to find ∏_{i ∈ inputs} Q_i. Returned as double — its only consumer
        // multiplies it by `rate` to form a propensity, so the value was always
        // a double-valued thing semantically.
        [[nodiscard]] double input_product(const ReactantList& inputs) const;

        // True iff every (non-env) input has enough of its species to fire.
        [[nodiscard]] bool can_fire(const Reaction& reaction) const;

        // Apply a fired reaction: -1 per input, +1 per product.
        // Environment reactants (env) carry no count and are skipped on either side.
        void update_counts(const Reaction& reaction);

        const Vessel& vessel;        // Shared, immutable: rules + symbol table.
        double t = 0;                // Simulation time, advanced by each delay.

        std::vector<std::size_t> state;  // Live species counts. Parallel to vessel.species()
        std::vector<double> delays;      // Delays. Parallel to vessel.reactions()
        std::mt19937 generator;          // Owned per Simulator so parallel runs don't share state.
    };
}

#endif //MINIPROJECT_SIMULATOR_HPP
