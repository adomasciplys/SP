#ifndef MINIPROJECT_REACTION_HPP
#define MINIPROJECT_REACTION_HPP

#include "partial_reaction.hpp"
#include "reactant.hpp"
#include "reactant_list.hpp"

namespace stochastic {

// (inputs >> rate) >>= products  -> a complete reaction rule
struct Reaction
{
    ReactantList inputs;
    ReactantList products;
    double rate;
};

Reaction operator>>=(PartialReaction pr, Reactant product);
Reaction operator>>=(PartialReaction pr, ReactantList products);

}  // namespace stochastic

#endif  // MINIPROJECT_REACTION_HPP
