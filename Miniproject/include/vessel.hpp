#ifndef MINIPROJECT_VESSEL_HPP
#define MINIPROJECT_VESSEL_HPP

#include "reactant.hpp"
#include "reaction.hpp"

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

namespace stochastic {

// A vessel collects the species and reactions that make up a stochastic system.
struct Vessel
{
    explicit Vessel(std::string name);

    [[nodiscard]] const std::string& name() const noexcept { return _name; }

    // The "void" sink/source used in decay reactions like A >> delta >>= env.
    [[nodiscard]] Reactant environment() const;

    // Register a species. Throws std::invalid_argument if `name` is already taken.
    Reactant add(std::string name, std::size_t initial_count);

    // Register a reaction rule.
    void add(Reaction r);

    [[nodiscard]] const std::vector<Reactant>& species() const noexcept { return _species; }
    [[nodiscard]] const std::vector<Reaction>& reactions() const noexcept { return _reactions; }

private:
    std::string _name;
    std::vector<Reactant> _species;
    std::vector<Reaction> _reactions;
    std::unordered_map<std::string, std::size_t> _index;
};

}  // namespace stochastic

#endif  // MINIPROJECT_VESSEL_HPP
