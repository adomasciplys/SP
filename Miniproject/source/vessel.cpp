#include "vessel.hpp"

#include <stdexcept>
#include <utility>

namespace stochastic {

Vessel::Vessel(std::string name) : _name(std::move(name)) {}

Reactant Vessel::environment() const
{
    return Reactant{"", 0};
}

Reactant Vessel::add(std::string name, std::size_t initial_count)
{
    if (_index.find(name) != _index.end())
        throw std::invalid_argument("reactant '" + name + "' already exists");

    Reactant r{name, initial_count};
    _index.emplace(name, _species.size());
    _species.push_back(r);
    return r;
}

void Vessel::add(Reaction r)
{
    _reactions.push_back(std::move(r));
}

}  // namespace stochastic
