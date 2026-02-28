#ifndef CALCULATOR_VISITOR_HPP
#define CALCULATOR_VISITOR_HPP

namespace calculator
{
    // Forward declarations
    struct expr_t;
    class var_t;
    struct const_t;
    struct unary_t;
    struct binary_t;
    struct assign_t;

    /** Visitor interface for the visitor pattern */
    struct Visitor
    {
        virtual void visit(const expr_t&) = 0;
        virtual void visit(const var_t&) = 0;
        virtual void visit(const const_t&) = 0;
        virtual void visit(const unary_t&) = 0;
        virtual void visit(const binary_t&) = 0;
        virtual void visit(const assign_t&) = 0;
        virtual ~Visitor() noexcept = default;
    };
}

#endif // CALCULATOR_VISITOR_HPP

