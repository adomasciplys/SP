#include "reaction.hpp"

#include <utility>

namespace stochastic {

Reaction operator>>=(PartialReaction pr, Reactant product)
{
    return Reaction{std::move(pr.inputs), ReactantList{{std::move(product)}}, pr.rate};
}

Reaction operator>>=(PartialReaction pr, ReactantList products)
{
    return Reaction{std::move(pr.inputs), std::move(products), pr.rate};
}

}  // namespace stochastic
