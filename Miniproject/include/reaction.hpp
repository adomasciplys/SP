#ifndef MINIPROJECT_REACTION_HPP
#define MINIPROJECT_REACTION_HPP

#include "rated_reactants.hpp"
#include "reactant_list.hpp"
#include "visitor.hpp"

#include <string>

namespace stochastic {

// A complete reaction rule: inputs -- (rate) --> products.
// Built by '>>=' from a RatedReactants and the products on the right hand side.
//
// Full chain for (A + B) >> rate >>= C + D:
//   1. 'A + B'               -> ReactantList   (operator+ in reactant_list.hpp)
//   2. '... >> rate'         -> RatedReactants (operator>> in rated_reactants.hpp)
//   3. '... >>= C + D'       -> Reaction        (operator>>= below)
//
// Operator precedence makes this parse without extra parenthesis:
// '+' binds tighter than '>>', which binds tighter than '>>='.
struct Reaction
{
    ReactantList inputs;
    ReactantList products;
    double rate;

    // A species is a catalyst when it appears on both sides of this reaction.
    // Consumed as an input and produced again, with no net change in count.
    [[nodiscard]] bool is_catalyst(const std::string& name) const
    {
        return inputs.contains(name) && products.contains(name);
    }

    void accept(Visitor& v) const { v.visit(*this); }
};

// '... >>= X'        -> single product on the right hand side.
Reaction operator>>=(RatedReactants pr, Reactant product);

// '... >>= X + Y'    -> multiple products on the right hand side.
Reaction operator>>=(RatedReactants pr, ReactantList products);

}  // namespace stochastic

#endif  // MINIPROJECT_REACTION_HPP
