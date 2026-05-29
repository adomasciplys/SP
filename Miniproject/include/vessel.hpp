#ifndef MINIPROJECT_VESSEL_HPP
#define MINIPROJECT_VESSEL_HPP

#include "reactant.hpp"
#include "reaction.hpp"
#include "symbol_table.hpp"
#include "visitor.hpp"

#include <cstddef>
#include <string>
#include <vector>

namespace stochastic {

// The container for one stochastic system. Holds:
//    - The set of species (reactants).
//    - The reaction rules that act on them.
struct Vessel
{
    explicit Vessel(std::string name);

    // Name of the system
    [[nodiscard]] const std::string& name() const noexcept { return _name; }

    // Represents the decay into the environment (A >>= env)
    [[nodiscard]] Reactant environment() const;

    // Register a new species with its initial count
    // Throws std::invalid_argument if a species with this name already exists.
    Reactant add(std::string name, std::size_t initial_count);

    // Register a reaction rule (built with '+', '>>', '>>=').
    void add(Reaction r);

    // Look up a previously added species' position in _species by name.
    // Throws std::out_of_range if the name was never registered.
    [[nodiscard]] std::size_t find_index(const std::string& name) const;

    [[nodiscard]] const std::vector<Reactant>& species() const noexcept { return _species; }
    [[nodiscard]] const std::vector<Reaction>& reactions() const noexcept { return _reactions; }

    void accept(Visitor& v) const { v.visit(*this); }

private:
    std::string _name;                    // system name
    std::vector<Reactant> _species;       // species in insertion order
    std::vector<Reaction> _reactions;     // reaction rules in insertion order
    SymbolTable<std::string, std::size_t> _index;  // name -> position in _species
};

}  // namespace stochastic

#endif  // MINIPROJECT_VESSEL_HPP
