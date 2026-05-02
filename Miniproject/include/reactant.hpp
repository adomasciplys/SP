#ifndef MINIPROJECT_REACTANT_HPP
#define MINIPROJECT_REACTANT_HPP

#include <cstddef>
#include <string>

namespace stochastic {

// A species in the reaction network. Identity is by name.
// An empty name marks the "void" (∅) used in decay reactions like
// `A >> rate >>= env`, where the reactant is consumed but no real species
// is produced. The simulator skips count updates for it.
struct Reactant
{
    std::string name;
    std::size_t initial_count{0};

    [[nodiscard]] bool is_environment() const noexcept { return name.empty(); }
};

}  // namespace stochastic

#endif  // MINIPROJECT_REACTANT_HPP
