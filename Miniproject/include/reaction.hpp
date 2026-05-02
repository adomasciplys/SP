#ifndef MINIPROJECT_REACTION_HPP
#define MINIPROJECT_REACTION_HPP

#include "partial_reaction.hpp"
#include "reactant.hpp"
#include "reactant_list.hpp"

namespace stochastic {

// A complete reaction rule: `inputs --(rate)--> products`.
// Built by `>>=` from a PartialReaction and the products on the right-hand side.
//
// Full chain for `(A + B) >> rate >>= C + D`:
//   1. `A + B`               -> ReactantList   (operator+ in reactant_list.hpp)
//   2. `... >> rate`         -> PartialReaction (operator>> in partial_reaction.hpp)
//   3. `... >>= C + D`       -> Reaction        (operator>>= below)
//
// Operator precedence makes this parse without extra parens:
// `+` binds tighter than `>>`, which binds tighter than `>>=`.
struct Reaction
{
    ReactantList inputs;
    ReactantList products;
    double rate;
};

// `... >>= X`        -> single product on the right-hand side.
Reaction operator>>=(PartialReaction pr, Reactant product);

// `... >>= X + Y`    -> multiple products on the right-hand side.
Reaction operator>>=(PartialReaction pr, ReactantList products);

}  // namespace stochastic

#endif  // MINIPROJECT_REACTION_HPP
