#include "reactant_list.hpp"

#include <algorithm>
#include <utility>

namespace stochastic {

bool ReactantList::contains(const std::string& name) const
{
    return std::ranges::any_of(items,
                               [&](const Reactant& r) { return r.name == name; });
}

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
