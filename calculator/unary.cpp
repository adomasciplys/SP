#include "unary.hpp"

namespace calculator
{
    unary_t::unary_t(std::shared_ptr<term_t> operand, const op_t op) : _operand{std::move(operand)}, _op{op}
    {
    }

    std::shared_ptr<term_t> unary_t::operand() const
    {
        return _operand;
    }

    op_t unary_t::op() const { return  _op; }

    // Return value of term with operator applied to it
    double unary_t::operator()(state_t& s) const
    {
        if (!_operand)
            throw std::logic_error{"missing operand for unary operation"};

        // Select based on operator
        switch (_op)
        {
        case plus:
            return _operand->operator()(s); // Evaluate expression unchanged
        case minus:
            return -_operand->operator()(s); // Evaluate expression and negate
        default:
            throw std::logic_error{"unsupported unary operation"};
        }
    }

    void unary_t::accept(Visitor& v) const { v.visit(*this); }
}

