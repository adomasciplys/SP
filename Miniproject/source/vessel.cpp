#include "vessel.hpp"

#include <utility>

namespace stochastic {

// Move the name in. All other members default construct
Vessel::Vessel(std::string name) : _name(std::move(name)) {}

// Environment is represented by empty string
Reactant Vessel::environment() const
{
    return Reactant{"", 0};
}

// Register a new species
// Two containers stay in sync: _index[name] == position of this species in _species
Reactant Vessel::add(std::string name, std::size_t initial_count)
{
    // Insert first: if the name already exists, the symbol table throws std::invalid_argument
    _index.insert(name, _species.size());
    // Insert succeeded
    auto r = Reactant{std::move(name), initial_count};
    _species.push_back(r);
    return r;
}

// Register a reaction rule.
// Reactions have no names, so no symbol-table entry.
// Their order in _reactions becomes the index used by Simulator::delays
void Vessel::add(Reaction r)
{
    _reactions.push_back(std::move(r));
}

// name -> position in _species
std::size_t Vessel::find_index(const std::string& name) const
{
    return _index.lookup(name);
}

}  // namespace stochastic
