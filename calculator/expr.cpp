#include "expr.hpp"
#include "const.hpp"
#include "unary.hpp"
#include "binary.hpp"
#include "assign.hpp"
#include "evaluator.hpp"

namespace calculator
{
    // If we get a term we simply move
    expr_t::expr_t(std::shared_ptr<term_t> term) : term{std::move(term)}
    {
    }

    // Implicit conversion from var_t
    expr_t::expr_t(const var_t& v) : term{std::make_shared<var_t>(v)}
    {
    }

    // Implicit conversion from double
    expr_t::expr_t(double value) : term{std::make_shared<const_t>(value)}
    {
    }

    // Constructor for unary operations
    expr_t::expr_t(const expr_t& e, op_t op)
        : term{std::make_shared<unary_t>(e.term, op)}
    {
    }

    // Constructor for binary operations
    expr_t::expr_t(const expr_t& e1, const expr_t& e2, op_t op)
        : term{std::make_shared<binary_t>(e1.term, e2.term, op)}
    {
    }

    // Constructor for assignment operations
    expr_t::expr_t(const var_t& v, const expr_t& e)
        : term{std::make_shared<assign_t>(std::make_shared<var_t>(v), e.term, op_t::assign)}
    {
    }

    // Constructor for assignment operations with a specific operator
    expr_t::expr_t(const var_t& v, const expr_t& e, op_t op)
        : term{std::make_shared<assign_t>(std::make_shared<var_t>(v), e.term, op)}
    {
    }

    double expr_t::operator()(state_t& s) const {
        Evaluator evaluator{0.0, s};
        term->accept(evaluator);
        return evaluator.result();
    }
}

