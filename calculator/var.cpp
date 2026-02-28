#include "var.hpp"

namespace calculator
{
    var_t::var_t(size_t id) : id{id}
    {
    }

    /** returns the value of the variable stored in a state */
    double var_t::operator()(state_t& s) const { return s[id]; }

    void var_t::accept(Visitor& v) const { v.visit(*this); }
}

