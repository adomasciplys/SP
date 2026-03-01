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

    double binary_t::operator()(state_t& s) const
    {
        if (!_term1)
            throw std::logic_error{"missing term 1"};
        if (!_term2)
            throw std::logic_error{"missing term 2"};

        double val1 = _term1->operator()(s); // Evaluate first term
        double val2 = _term2->operator()(s); // Evaluate second term

        switch (_op)
        {
        case add:
            return val1 + val2;
        case sub:
            return val1 - val2;
        case mult:
            return val1 * val2;
        case div:
            return safe_divide(val1, val2);
        default:
            throw std::logic_error{"unsupported binary operation"};
        }
    }

    void binary_t::accept(Visitor& v) const { v.visit(*this); }
}

