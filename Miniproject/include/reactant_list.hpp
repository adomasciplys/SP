#ifndef MINIPROJECT_REACTANT_LIST_HPP
#define MINIPROJECT_REACTANT_LIST_HPP

#include "reactant.hpp"

#include <vector>

namespace stochastic {

// A sum of reactants used on either side of a reaction (e.g. `A + B + C`).
// Built up by the `operator+` overloads below as the user types reaction rules.
struct ReactantList
{
    std::vector<Reactant> items;
};

// `A + B`        -> a new list of two reactants.
ReactantList operator+(Reactant a, Reactant b);

// `(A + B) + C`  -> appends C to an existing list.
ReactantList operator+(ReactantList list, Reactant r);

// `A + (B + C)`  -> prepends A to an existing list (rare, but keeps `+` symmetric).
ReactantList operator+(Reactant r, ReactantList list);

}  // namespace stochastic

#endif  // MINIPROJECT_REACTANT_LIST_HPP
