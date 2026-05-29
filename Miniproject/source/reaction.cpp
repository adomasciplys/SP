#include "reaction.hpp"

#include <utility>

namespace stochastic {

Reaction operator>>=(RatedReactants pr, Reactant product)
{
    return Reaction{std::move(pr.inputs), ReactantList{{std::move(product)}}, pr.rate};
}

Reaction operator>>=(RatedReactants pr, ReactantList products)
{
    return Reaction{std::move(pr.inputs), std::move(products), pr.rate};
}

}  // namespace stochastic
