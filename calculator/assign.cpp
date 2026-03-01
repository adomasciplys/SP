#include "assign.hpp"
#include "var.hpp"

namespace calculator
{
    assign_t::assign_t(std::shared_ptr<var_t> var, std::shared_ptr<term_t> val, const op_t op)
        : _var{std::move(var)}, _term{std::move(val)}, _op{op}
    {
    }

    std::shared_ptr<var_t> assign_t::var() const
    {
        return _var;
    }

    std::shared_ptr<term_t> assign_t::term() const
    {
        return _term;
    }

    op_t assign_t::op() const
    {
        return _op;
    }

    double assign_t::operator()(state_t& s) const
    {
        if (!_var)
            throw std::logic_error{"missing variable"};
        if (!_term)
            throw std::logic_error{"missing term"};

        const double rhs = _term->operator()(s); // Evaluate rhs
        // Get a reference to var in state where we want to store the new value
        double& lhs = s[_var->_id];

        switch (_op)
        {
        case assign:
            return lhs = rhs;
        case add:
            return lhs = lhs + rhs; // +=
        case sub:
            return lhs = lhs - rhs; // -=
        case mult:
            return lhs = lhs * rhs; // *=
        case div:
            return lhs = safe_divide(lhs, rhs); // /=
        default:
            throw std::logic_error{"unsupported assignment operation"};
        }
    }

    void assign_t::accept(Visitor& v) const { v.visit(*this); }
}

