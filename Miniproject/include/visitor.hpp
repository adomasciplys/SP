#ifndef MINIPROJECT_VISITOR_HPP
#define MINIPROJECT_VISITOR_HPP

namespace stochastic {

struct Reactant;
struct Reaction;
struct Vessel;

// Visitor interface for traversing a reaction network.
// Concrete visitors override visit() for the elements they care about.
struct Visitor
{
    virtual void visit(const Reactant&) = 0;
    virtual void visit(const Reaction&) = 0;
    virtual void visit(const Vessel&) = 0;

    virtual ~Visitor() noexcept = default;
};

}  // namespace stochastic

#endif  // MINIPROJECT_VISITOR_HPP
