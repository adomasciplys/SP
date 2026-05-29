#include "rated_reactants.hpp"
#include <utility>

namespace stochastic {

RatedReactants operator>>(Reactant r, double rate)
{
    return RatedReactants{ReactantList{{std::move(r)}}, rate};
}

RatedReactants operator>>(ReactantList list, double rate)
{
    return RatedReactants{std::move(list), rate};
}

}  // namespace stochastic
