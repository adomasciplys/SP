#include "unary.hpp"

namespace calculator
{
    unary_t::unary_t(std::shared_ptr<term_t> operand, const op_t op) : operand{std::move(operand)}, op{op}
    {
    }

    void unary_t::accept(Visitor& v) const { v.visit(*this); }
}
