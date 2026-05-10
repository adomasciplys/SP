#ifndef MINIPROJECT_SIMULATOR_HPP
#define MINIPROJECT_SIMULATOR_HPP

#include "vessel.hpp"
#include <vector>
#include <random>
#include <algorithm>
#include <limits>

namespace stochastic
{
    struct Simulator
    {
        double t = 0;

        // Parallel to vessel.species()
        // Holds the amount of each input and output reactant
        std::vector<std::size_t> counts;

        Simulator(const Vessel& vessel, std::size_t seed) : vessel(vessel),
                                                            counts(std::vector<std::size_t>(vessel.species().size(), 0)),
                                                            delays(std::vector<double>(vessel.reactions().size(), 0)),
                                                            generator(seed)
        {
        }

        void simulate(double end_time)
        {
            while (t < end_time)
            {
                compute_delays(); // // foreach r ∈ R do Compute r.delay;
                // r∶= argminr∈R r.delay;
                auto it_r_min = std::min_element(delays.begin(), delays.end());
                std::size_t idx = std::distance(delays.begin(), it_r_min); // index of fired reaction
                const Reaction& fired_reaction = vessel.reactions()[idx];

                // t∶= t + r.delay;
                t += delays[idx];

                if (check_if_enough_input_reactants(fired_reaction))
                {
                    update_counts(fired_reaction);
                }
                // TODO: Print/store/observe the state ⟨Qi⟩;
            }
        }

        void compute_delays()
        {
            const auto& reactions = vessel.reactions();
            for (std::size_t i = 0; i < reactions.size(); ++i)
            {
                const Reaction& reaction = reactions[i];
                const double lambda = reaction.rate * input_product(reaction.inputs);
                // exponential_distribution{0} is UB; an unfireable reaction
                // gets an "infinite" delay so min_element naturally skips it.
                if (lambda == 0.0)
                {
                    delays[i] = std::numeric_limits<double>::infinity();
                    continue;
                }
                std::exponential_distribution<> distribution(lambda);
                delays[i] = distribution(generator);
            }
        }

        [[nodiscard]] std::size_t input_product(const ReactantList& inputs) const
        {
            std::size_t product = 1;

            for (const auto& reactant : inputs.items)
            {
                if (reactant.is_environment()) continue;
                product *= counts[vessel.find_index(reactant.name)];
            }
            return product;
        };

        [[nodiscard]] bool check_if_enough_input_reactants(const Reaction& reaction) const
        {
            // Decrement a working copy as we go
            auto remaining = counts;
            for (const auto& reactant : reaction.inputs.items)
            {
                if (reactant.is_environment()) continue;
                const auto idx = vessel.find_index(reactant.name);
                if (remaining[idx] == 0) return false;
                --remaining[idx];
            }
            return true;
        }

        void update_counts(const Reaction& reaction)
        {
            for (const auto& reactant : reaction.inputs.items)
            {
                if (reactant.is_environment()) continue;
                auto idx = vessel.find_index(reactant.name);
                counts[idx]--; // Use inputs
            }

            for (const auto& reactant : reaction.products.items)
            {
                if (reactant.is_environment()) continue;
                auto idx = vessel.find_index(reactant.name);
                counts[idx]++; // Increment products
            }
        }

    private:
        Vessel& vessel;
        std::vector<double> delays; // Parallel to vessel.reactions()
        std::mt19937 generator;
    };
}

#endif //MINIPROJECT_SIMULATOR_HPP
