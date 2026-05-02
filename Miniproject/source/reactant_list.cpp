#include "reactant_list.hpp"

#include <utility>

namespace stochastic {

ReactantList operator+(Reactant a, Reactant b)
{
    return ReactantList{{std::move(a), std::move(b)}};
}

ReactantList operator+(ReactantList list, Reactant r)
{
    list.items.push_back(std::move(r));
    return list;
}

ReactantList operator+(Reactant r, ReactantList list)
{
    list.items.insert(list.items.begin(), std::move(r));
    return list;
}

}  // namespace stochastic
