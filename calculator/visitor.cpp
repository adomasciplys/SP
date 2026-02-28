#include "calculator.hpp"

struct Visitor {
    virtual void visit(const expr_t&) = 0;
    virtual void visit(const var_t&) = 0;
    virtual void visit(const const_t&) = 0;
    virtual void visit(const unary_t&) = 0;
    virtual void visit(const binary_t&) = 0;
    virtual void visit(const assign_t&) = 0;
    virtual ~Visitor() noexcept = default;
};