#ifndef MINIPROJECT_REACTANT_LIST_HPP
#define MINIPROJECT_REACTANT_LIST_HPP

#include "reactant.hpp"

#include <string>
#include <vector>

namespace stochastic {

// A sum of reactants used on either side of a reaction (e.g. 'A + B + C').
// Built up by the 'operator' overloads
struct ReactantList
{
    std::vector<Reactant> reactants;

    // True iff any of the reactants have the given name.
    [[nodiscard]] bool contains(const std::string& name) const;
};

// 'A + B'        -> a new list of two reactants.
ReactantList operator+(Reactant a, Reactant b);

// '(A + B) + C'  -> appends C to an existing list (this is not used, just here for completeness of '+')
ReactantList operator+(ReactantList list, Reactant r);

// 'A + (B + C)'  -> prepends A to an existing list (for the sake of associativity of '+').
ReactantList operator+(Reactant r, ReactantList list);

}  // namespace stochastic

#endif  // MINIPROJECT_REACTANT_LIST_HPP
