#ifndef MINIPROJECT_PARTIAL_REACTION_HPP
#define MINIPROJECT_PARTIAL_REACTION_HPP

#include "reactant.hpp"
#include "reactant_list.hpp"

namespace stochastic {

// inputs >> rate  -> a partial reaction waiting for products via >>=
struct PartialReaction
{
    ReactantList inputs;
    double rate;
};

PartialReaction operator>>(Reactant r, double rate);
PartialReaction operator>>(ReactantList list, double rate);

}  // namespace stochastic

#endif  // MINIPROJECT_PARTIAL_REACTION_HPP
