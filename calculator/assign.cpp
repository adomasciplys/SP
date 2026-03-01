#include "assign.hpp"
#include "var.hpp"

namespace calculator
{
    assign_t::assign_t(std::shared_ptr<var_t> var, std::shared_ptr<term_t> val, const op_t op)
        : var{std::move(var)}, term{std::move(val)}, op{op}
    {
    }

    void assign_t::accept(Visitor& v) const { v.visit(*this); }
}
