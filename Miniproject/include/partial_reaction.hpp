#ifndef MINIPROJECT_PARTIAL_REACTION_HPP
#define MINIPROJECT_PARTIAL_REACTION_HPP

#include "reactant.hpp"
#include "reactant_list.hpp"

namespace stochastic {

// An intermediate result in the reaction DSL: the inputs and rate are known,
// but the products have not been supplied yet.
// Produced by `>>` and consumed by `>>=`
//
// Example: in `(A + B) >> rate >>= C`, the sub-expression `(A + B) >> rate`
// evaluates to a PartialReaction that is then completed with `>>= C`.

struct PartialReaction
{
    ReactantList inputs;
    double rate;
};

// `A >> rate`        -> single-reactant input lifted into a PartialReaction.
PartialReaction operator>>(Reactant r, double rate);

// `(A + B) >> rate`  -> multi-reactant input attached to a rate.
PartialReaction operator>>(ReactantList list, double rate);

}  // namespace stochastic

#endif  // MINIPROJECT_PARTIAL_REACTION_HPP
