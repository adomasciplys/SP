#ifndef MINIPROJECT_SIMULATOR_HPP
#define MINIPROJECT_SIMULATOR_HPP

#include "vessel.hpp"
#include <vector>
#include <random>
#include <algorithm>

namespace stochastic
{
    struct Simulator
    {
        Vessel& vessel;
        double t = 0;
        // Parallel to vessel.species() (so counts[i] is the current count of species i)
        std::vector<std::size_t> counts;
        std::vector<double> delays; // Parallel to vessel.reactions()
        std::mt19937 generator;

        Simulator(Vessel& vessel, std::size_t seed) : vessel(vessel),
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
                if (it_r_min == delays.end()) break;
                std::size_t idx = std::distance(delays.begin(), it_r_min); // index of chosen reaction
                const Reaction& fired_reaction = vessel.reactions().at(idx);

                // t∶= t + r.delay;
                t += delays.at(idx);


            }
        }

        void compute_delays()
        {
            const auto& reactions = vessel.reactions();
            const auto& species = vessel.species();
            std::size_t i = 0;
            for (const Reaction& reaction : reactions)
            {
                std::exponential_distribution<> distribution(reaction.rate * input_product(reaction.inputs));
                delays[i] =  distribution(generator);
                ++i;
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

    };
}

#endif //MINIPROJECT_SIMULATOR_HPP
