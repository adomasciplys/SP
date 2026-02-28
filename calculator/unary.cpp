#include "unary.hpp"

namespace calculator
{
    unary_t::unary_t(std::shared_ptr<term_t> operand, const op_t op) : operand{std::move(operand)}, op{op}
    {
    }

    // Return value of term with operator applied to it
    double unary_t::operator()(state_t& s) const
    {
        if (!operand)
            throw std::logic_error{"missing operand for unary operation"};

        // Select based on operator
        switch (op)
        {
        case plus:
            return operand->operator()(s); // Evaluate expression unchanged
        case minus:
            return -operand->operator()(s); // Evaluate expression and negate
        default:
            throw std::logic_error{"unsupported unary operation"};
        }
    }

    void unary_t::accept(Visitor& v) const { v.visit(*this); }
}

