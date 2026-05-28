#include "simulator.hpp"

#include <algorithm>
#include <cmath>
#include <iterator>
#include <limits>
#include <numeric>

namespace stochastic
{
    Simulator::Simulator(const Vessel& vessel, std::size_t seed)
        : vessel(vessel),
          state(vessel.species().size()),
          delays(vessel.reactions().size()),
          generator(seed)
    {
        // `state` is seeded from each species' initial_count.
        std::ranges::transform(vessel.species(), state.begin(),
                               [](const Reactant& r) { return r.initial_count; });
    }

    void Simulator::simulate(double end_time)
    {
        while (t < end_time) step();
    }

    Generator<Simulator::Sample> Simulator::run(double end_time)
    {
        co_yield Sample{time(), counts()};  // initial state at t = 0
        while (t < end_time)
        {
            step();
            if (!std::isfinite(t)) break;
            co_yield Sample{time(), counts()};
        }
    }

    void Simulator::step()
    {
        // Line 3: foreach r ∈ R do Compute r.delay;
        compute_delays();

        // Line 4: r∶= argminr∈R r.delay;
        const auto idx = static_cast<std::size_t>(
            std::distance(delays.begin(), std::ranges::min_element(delays)));
        const auto& fired_reaction = vessel.reactions()[idx];

        // Line 5: t∶= t + r.delay;
        t += delays[idx];

        // Line 6: only apply the firing when every input is present.
        if (can_fire(fired_reaction))
        {
            // Line 8-9: decrement inputs, increment products.
            update_counts(fired_reaction);
        }
        // TODO: Print/store/observe the state ⟨Qi⟩;
    }

    void Simulator::compute_delays()
    {
        std::ranges::transform(vessel.reactions(), delays.begin(),
            [&](const Reaction& r) -> double {
                const auto lambda = r.rate * static_cast<double>(input_product(r.inputs));
                if (lambda == 0.0) return std::numeric_limits<double>::infinity();
                auto dist = std::exponential_distribution<>{lambda};
                return dist(generator);
            });
    }

    std::size_t Simulator::input_product(const ReactantList& inputs) const
    {
        return std::accumulate(inputs.items.begin(), inputs.items.end(), std::size_t{1},
            [&](std::size_t acc, const Reactant& r) {
                return r.is_environment() ? acc : acc * state[vessel.find_index(r.name)];
            });
    }

    bool Simulator::can_fire(const Reaction& reaction) const
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

    void Simulator::update_counts(const Reaction& reaction)
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
}
