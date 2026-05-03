#include "vessel.hpp"

#include <utility>

namespace stochastic {

Vessel::Vessel(std::string name) : _name(std::move(name)) {}

Reactant Vessel::environment() const
{
    return Reactant{"", 0};
}

Reactant Vessel::add(std::string name, std::size_t initial_count)
{
    // Symbol table enforces uniqueness; throws std::invalid_argument on duplicate.
    _index.insert(name, _species.size());
    Reactant r{name, initial_count};
    _species.push_back(r);
    return r;
}

void Vessel::add(Reaction r)
{
    _reactions.push_back(std::move(r));
}

const Reactant& Vessel::find(const std::string& name) const
{
    // Symbol table throws std::out_of_range if `name` is not registered.
    return _species.at(_index.lookup(name));
}

}  // namespace stochastic
