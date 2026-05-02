#ifndef MINIPROJECT_REACTANT_LIST_HPP
#define MINIPROJECT_REACTANT_LIST_HPP

#include "reactant.hpp"

#include <vector>

namespace stochastic {

// A sum of reactants on either side of a reaction (e.g. A + B + C).
struct ReactantList
{
    std::vector<Reactant> items;
};

ReactantList operator+(Reactant a, Reactant b);
ReactantList operator+(ReactantList list, Reactant r);
ReactantList operator+(Reactant r, ReactantList list);

}  // namespace stochastic

#endif  // MINIPROJECT_REACTANT_LIST_HPP
