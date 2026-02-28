#include "assign.hpp"
#include "var.hpp"

namespace calculator
{
    assign_t::assign_t(std::shared_ptr<var_t> var, std::shared_ptr<term_t> val, const op_t op)
        : var{std::move(var)}, term{std::move(val)}, op{op}
    {
    }

    double assign_t::operator()(state_t& s) const
    {
        if (!var)
            throw std::logic_error{"missing variable"};
        if (!term)
            throw std::logic_error{"missing term"};

        const double rhs = term->operator()(s); // Evaluate rhs
        // Get a reference to var in state where we want to store the new value
        double& lhs = s[var->id];

        switch (op)
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

