#ifndef MINIPROJECT_SIMULATOR_HPP
#define MINIPROJECT_SIMULATOR_HPP

#include "vessel.hpp"

#include <cstddef>
#include <random>
#include <vector>

namespace stochastic
{
    // Stochastic simulator (Algorithm 1)
    // Borrows an immutable Vessel and owns the state + RNG, so
    // independent simulations can run against the same Vessel concurrently.
    struct Simulator
    {
        Simulator(const Vessel& vessel, std::size_t seed);

        // Run Algorithm 1 forward until t >= end_time.
        void simulate(double end_time);

        // One iteration of Algorithm 1.
        // The firing of the chosen reaction is gated on having enough of each input species available.
        void step();

        [[nodiscard]] double time() const noexcept { return t; }
        [[nodiscard]] const std::vector<std::size_t>& counts() const noexcept { return state; }

    private:
        // Use Equation 1, to compute the delays for each reaction
        void compute_delays();

        // Helper to find ∏i ∈ r.inputs Qi
        [[nodiscard]] std::size_t input_product(const ReactantList& inputs) const;

        // True iff every (non-env) input has enough of its species to fire.
        [[nodiscard]] bool can_fire(const Reaction& reaction) const;

        // Apply a fired reaction: -1 per input, +1 per product.
        // Environment reactants (∅) carry no count and are skipped on either side.
        void update_counts(const Reaction& reaction);

        const Vessel& vessel;        // Shared, immutable: rules + symbol table.
        double t = 0;                // Simulation time, advanced by each delay.
        // Live species counts. Parallel to vessel.species()
        // state[i] is the current count of the i-th species
        std::vector<std::size_t> state;
        std::vector<double> delays;  // Parallel to vessel.reactions()
        std::mt19937 generator;      // Owned per Simulator so parallel runs don't share state.
    };
}

#endif //MINIPROJECT_SIMULATOR_HPP
