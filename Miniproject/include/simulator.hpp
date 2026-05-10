#ifndef MINIPROJECT_SIMULATOR_HPP
#define MINIPROJECT_SIMULATOR_HPP

#include "vessel.hpp"
#include <vector>
#include <random>
#include <algorithm>
#include <limits>
#include <numeric>

namespace stochastic
{
    // Stochastic simulator (Algorithm 1)
    // Borrows an immutable Vessel and owns the state + RNG, so
    // independent simulations can run against the same Vessel concurrently.
    struct Simulator
    {
        double t = 0;  // Simulation time, advanced by each delay.

        // Live species counts. Parallel to vessel.species()
        // state[i] is the current count of the i-th species
        std::vector<std::size_t> state;

        Simulator(const Vessel& vessel, std::size_t seed)
            : state(vessel.species().size()),
              vessel(vessel),
              delays(vessel.reactions().size()),
              generator(seed)
        {
            // `state` is seeded from each species' initial_count.
            std::ranges::transform(vessel.species(), state.begin(),
                                   [](const Reactant& r) { return r.initial_count; });
        }

        // Run Algorithm 1 forward until t >= end_time.
        // Time always advances on each step
        // The firing of the chosen reaction is gated on having enough of each input species available.
        void simulate(double end_time)
        {
            while (t < end_time)
            {
                // Line 3: foreach r ∈ R do Compute r.delay;
                compute_delays();

                // Line 4: r∶= argminr∈R r.delay;
                const auto it_r_min = std::ranges::min_element(delays);
                const std::size_t idx = std::distance(delays.begin(), it_r_min); // index of fired reaction
                const Reaction& fired_reaction = vessel.reactions()[idx];

                // Line 5: t∶= t + r.delay;
                t += delays[idx];

                // Line 6: only apply the firing when every input is present.
                if (check_if_enough_input_reactants(fired_reaction))
                {
                    // Line 8-9: decrement inputs, increment products.
                    update_counts(fired_reaction);
                }
                // TODO: Print/store/observe the state ⟨Qi⟩;
            }
        }

        // Use Equation 1, to compute the delays for each reaction
        void compute_delays()
        {
            std::ranges::transform(vessel.reactions(), delays.begin(),
                [&](const Reaction& r) -> double {
                    const double lambda = r.rate * input_product(r.inputs);
                    if (lambda == 0.0) return std::numeric_limits<double>::infinity();
                    std::exponential_distribution<> dist(lambda);
                    return dist(generator);
                });
        }

        // Helper to find ∏i ∈ r.inputs Qi
        [[nodiscard]] std::size_t input_product(const ReactantList& inputs) const
        {
            return std::accumulate(inputs.items.begin(), inputs.items.end(), std::size_t{1},
                [&](std::size_t acc, const Reactant& r) {
                    return r.is_environment() ? acc : acc * state[vessel.find_index(r.name)];
                });
        }

        // True iff every (non-env) input has enough of its species to fire.
        [[nodiscard]] bool check_if_enough_input_reactants(const Reaction& reaction) const
        {
            auto remaining = state;
            for (const auto& reactant : reaction.inputs.items)
            {
                if (reactant.is_environment()) continue;
                const auto idx = vessel.find_index(reactant.name);
                if (remaining[idx] == 0) return false;
                --remaining[idx];
            }
            return true;
        }

        // Apply a fired reaction: -1 per input, +1 per product.
        // Environment reactants (∅) carry no count and are skipped on either side.
        void update_counts(const Reaction& reaction)
        {
            auto apply_delta = [&](const ReactantList& list, int delta) {
                for (const auto& r : list.items) {
                    if (r.is_environment()) continue;
                    state[vessel.find_index(r.name)] += delta;
                }
            };
            apply_delta(reaction.inputs,   -1);
            apply_delta(reaction.products, +1);
        }

    private:
        const Vessel& vessel;        // Shared, immutable: rules + symbol table.
        std::vector<double> delays;  // Parallel to vessel.reactions()
        std::mt19937 generator;      // Owned per Simulator so parallel runs don't share state.
    };
}

#endif //MINIPROJECT_SIMULATOR_HPP
