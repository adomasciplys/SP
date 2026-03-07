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

    void unary_t::accept(Visitor& v) const { v.visit(*this); }
}

