#include "simulator.hpp"

#include <algorithm>
#include <cmath>
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
        // 'state' is seeded from each species' initial_count, parallel to vessel.species().
        std::ranges::transform(vessel.species(), state.begin(),
                               [](const Reactant& r) { return r.initial_count; });
    }

    // Step until simulation time reaches end_time. No observation.
    void Simulator::simulate(double end_time)
    {
        while (t < end_time) step();
    }

    // Streaming variant. Yields a Sample at t = 0, then one per step.
    // Stops when t >= end_time or t becomes non-finite (every reaction unfireable).
    Generator<Simulator::Sample> Simulator::run(double end_time)
    {
        co_yield Sample{time(), counts()};  // initial state at t = 0
        while (t < end_time)
        {
            step();
            if (!std::isfinite(t)) break;  // every reaction unfireable
            co_yield Sample{time(), counts()};
        }
    }

    // One iteration of Algorithm 1 (Doob-Gillespie).
    void Simulator::step()
    {
        // Line 3: foreach r ∈ R do Compute r.delay;
        compute_delays();

        // Line 4: r := argmin_{r ∈ R} r.delay;
        // Work with the iterator directly so we don't have to convert the
        // distance to size_t for indexing.
        const auto fired_it = std::ranges::min_element(delays);
        const auto& fired_reaction = vessel.reactions()[fired_it - delays.begin()];

        // Line 5: t := t + r.delay;
        t += *fired_it;

        // Line 6: only apply the firing when every input is actually present.
        if (can_fire(fired_reaction))
        {
            // Line 8-9: -1 per input, +1 per product. Environment reactants are skipped.
            update_counts(fired_reaction);
        }
    }

    // Equation 1: for every reaction, draw a delay from Exp(rate * ∏ Q_i).
    // If lambda == 0 (some input has count 0), the reaction can't fire
    void Simulator::compute_delays()
    {
        std::ranges::transform(vessel.reactions(), delays.begin(),
            [&](const Reaction& r) -> double {
                const auto lambda = r.rate * input_product(r.inputs);
                if (lambda == 0.0) return std::numeric_limits<double>::infinity();
                auto dist = std::exponential_distribution<>{lambda};
                return dist(generator);
            });
    }

    // ∏_{i ∈ inputs} Q_i. Current population product.
    // Environment reactants are skipped.
    double Simulator::input_product(const ReactantList& inputs) const
    {
        return std::accumulate(inputs.reactants.begin(), inputs.reactants.end(), 1.0,
            [&](double acc, const Reactant& r) {
                return r.is_environment() ? acc : acc * state[vessel.find_index(r.name)];
            });
    }

    // True iff every (non-env) input has enough copies to fire
    // Decrements a local copy of 'state'.
    bool Simulator::can_fire(const Reaction& reaction) const
    {
        auto remaining = state;
        for (const auto& reactant : reaction.inputs.reactants)
        {
            if (reactant.is_environment()) continue;
            const auto idx = vessel.find_index(reactant.name);
            if (remaining[idx] == 0) return false;
            --remaining[idx];
        }
        return true;
    }

    // Apply a fired reaction: -1 per input, +1 per product.
    // Catalysts naturally cancel (-1 then +1).
    // Environment reactants are skipped
    void Simulator::update_counts(const Reaction& reaction)
    {
        auto apply_delta = [&](const ReactantList& list, int delta) {
            for (const auto& r : list.reactants) {
                if (r.is_environment()) continue;
                state[vessel.find_index(r.name)] += delta;
            }
        };
        apply_delta(reaction.inputs,   -1);
        apply_delta(reaction.products, +1);
    }
}
