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
        return _var; // It would just simplify the code if I would not have to write all these getters :D
    }

    std::shared_ptr<term_t> assign_t::term() const
    {
        return _term;
    }

    op_t assign_t::op() const
    {
        return _op;
    }

    void assign_t::accept(Visitor& v) const { v.visit(*this); }
}

