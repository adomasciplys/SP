#ifndef MINIPROJECT_REACTANT_HPP
#define MINIPROJECT_REACTANT_HPP

#include <cstddef>
#include <string>

namespace stochastic {

// A species in the reaction network. Identity is by name; the empty name marks
// the "void" environment used as a sink/source for decay reactions.
struct Reactant
{
    std::string name;
    std::size_t initial_count{0};

    [[nodiscard]] bool is_environment() const noexcept { return name.empty(); }
};

}  // namespace stochastic

#endif  // MINIPROJECT_REACTANT_HPP
