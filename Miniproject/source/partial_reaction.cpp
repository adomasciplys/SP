#include "partial_reaction.hpp"

#include <utility>

namespace stochastic {

PartialReaction operator>>(Reactant r, double rate)
{
    return PartialReaction{ReactantList{{std::move(r)}}, rate};
}

PartialReaction operator>>(ReactantList list, double rate)
{
    return PartialReaction{std::move(list), rate};
}

}  // namespace stochastic
