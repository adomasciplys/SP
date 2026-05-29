#ifndef MINIPROJECT_RATED_REACTANTS_HPP
#define MINIPROJECT_RATED_REACTANTS_HPP

#include "reactant.hpp"
#include "reactant_list.hpp"

namespace stochastic {

// An intermediate result in the reaction DSL:
//     - The inputs and rate are known, but the products have not been supplied yet.
// Produced by '>>' and consumed by '>>='
struct RatedReactants
{
    ReactantList inputs;
    double rate;
};

// 'A >> rate'        -> single reactant input lifted into a RatedReactants.
RatedReactants operator>>(Reactant r, double rate);

// '(A + B) >> rate'  -> multi reactant input attached to a rate.
RatedReactants operator>>(ReactantList list, double rate);

}  // namespace stochastic

#endif  // MINIPROJECT_RATED_REACTANTS_HPP
