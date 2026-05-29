#ifndef MINIPROJECT_REACTANT_HPP
#define MINIPROJECT_REACTANT_HPP

#include "visitor.hpp"

#include <cstddef>
#include <string>

namespace stochastic {

// A species in the reaction network.
// Identity is by name.
// An empty name marks the env used in decay reactions
struct Reactant
{
    std::string name;
    std::size_t initial_count{0};

    [[nodiscard]] bool is_environment() const noexcept { return name.empty(); }

    void accept(Visitor& v) const { v.visit(*this); }
};

}  // namespace stochastic

#endif  // MINIPROJECT_REACTANT_HPP
