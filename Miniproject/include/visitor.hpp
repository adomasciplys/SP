#ifndef MINIPROJECT_VISITOR_HPP
#define MINIPROJECT_VISITOR_HPP

namespace stochastic
{
    // Forward declarations
    struct PartialReaction;
    struct Reactant;
    struct ReactantList;
    struct Reaction;
    struct Vessel;

    /** Visitor interface for the visitor pattern */
    struct Visitor
    {
        virtual void visit(const PartialReaction) = 0;
        virtual void visit(const Reactant) = 0;
        virtual void visit(const ReactantList) = 0;
        virtual void visit(const Reaction) = 0;
        virtual void visit(const Vessel) = 0;

        virtual ~Visitor() noexcept = default;
    };
}

#endif //MINIPROJECT_VISITOR_HPP