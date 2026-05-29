#include "reactant_list.hpp"

#include <algorithm>
#include <utility>

namespace stochastic {

// Linear scan by name.
bool ReactantList::contains(const std::string& name) const
{
    return std::ranges::any_of(reactants,
                               [&](const Reactant& r) { return r.name == name; });
}

// 'A + B' start a new list with two reactants.
// Parameters are taken by value and moved into the vector
ReactantList operator+(Reactant a, Reactant b)
{
    return ReactantList{{std::move(a), std::move(b)}};
}

// '(A + B) + C': Grows an existing list on the right
ReactantList operator+(ReactantList list, Reactant r)
{
    list.reactants.push_back(std::move(r));
    return list;
}

// 'A + (B + C)': Grows an existing list on the left.
ReactantList operator+(Reactant r, ReactantList list)
{
    list.reactants.insert(list.reactants.begin(), std::move(r));
    return list;
}

}  // namespace stochastic
