#ifndef MINIPROJECT_VESSEL_HPP
#define MINIPROJECT_VESSEL_HPP

#include "reactant.hpp"
#include "reaction.hpp"

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

namespace stochastic {

// The container for one stochastic system holds:
//    - The set of species (reactants)
//    - The reaction rules that act on them.
// The user builds a Vessel by calling `add()` for each species and each reaction
// The simulator (Algorithm 1) then reads `species()` and `reactions()` to drive the run.

struct Vessel
{
    explicit Vessel(std::string name);

    // Human-readable name of the system, e.g. "Circadian Rhythm".
    [[nodiscard]] const std::string& name() const noexcept { return _name; }

    // Returns the "void" reactant (∅): a placeholder used on either side of a
    // reaction to model decay or spontaneous creation.
    [[nodiscard]] Reactant environment() const;

    // Register a new species with its initial count
    // and return a handle to use in reaction expressions.
    // Throws std::invalid_argument if a species with this name already exists.
    Reactant add(std::string name, std::size_t initial_count);

    // Register a reaction rule (built with `+`, `>>`, `>>=`).
    void add(Reaction r);

    // Read-only views used by the simulator and the graph-printing visitor.
    [[nodiscard]] const std::vector<Reactant>& species() const noexcept { return _species; }
    [[nodiscard]] const std::vector<Reaction>& reactions() const noexcept { return _reactions; }

private:
    std::string _name;
    std::vector<Reactant> _species;       // species in insertion order
    std::vector<Reaction> _reactions;     // reaction rules in insertion order
    std::unordered_map<std::string, std::size_t> _index;  // name -> position in _species
};

}  // namespace stochastic

#endif  // MINIPROJECT_VESSEL_HPP
