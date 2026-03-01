#include "binary.hpp"

namespace calculator
{
    binary_t::binary_t(std::shared_ptr<term_t> term1, std::shared_ptr<term_t> term2, const op_t op)
        : _term1{std::move(term1)}, _term2{std::move(term2)}, _op{op}
    {
    }

    std::shared_ptr<term_t> binary_t::term1() const
    {
        return _term1;
    }

    std::shared_ptr<term_t> binary_t::term2() const
    {
        return _term2;
    }

    op_t binary_t::op() const
    {
        return _op;
    }

    void binary_t::accept(Visitor& v) const { v.visit(*this); }
}

