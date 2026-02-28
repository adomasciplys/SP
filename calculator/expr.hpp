#ifndef CALCULATOR_EXPR_HPP
#define CALCULATOR_EXPR_HPP

#include "term.hpp"
#include "var.hpp"
#include "op.hpp"
#include <memory>

namespace calculator
{
    struct const_t;
    struct unary_t;
    struct binary_t;
    struct assign_t;

    /**
     * expr_t represents an expression and implements all operations.
     */
    struct expr_t
    {
        std::shared_ptr<term_t> term;
        friend struct Printer;

        // If we get a term we simply move
        expr_t(std::shared_ptr<term_t> term);

        // Implicit conversion from var_t
        expr_t(const var_t& v);

        // Implicit conversion from double
        expr_t(double value);

        // Constructor for unary operations
        expr_t(const expr_t& e, op_t op);

        // Constructor for binary operations
        expr_t(const expr_t& e1, const expr_t& e2, op_t op);

        // Constructor for assignment operations
        expr_t(const var_t& v, const expr_t& e);

        // Constructor for assignment operations with a specific operator
        expr_t(const var_t& v, const expr_t& e, op_t op);

        double operator()(state_t& s) const;
    };
}

#endif // CALCULATOR_EXPR_HPP

