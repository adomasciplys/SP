#include "var.hpp"

namespace calculator
{
    var_t::var_t(size_t id) : _id{id}
    {
    }

    /** returns the value of the variable stored in a state */
    size_t var_t::id() const { return _id; }

    void var_t::accept(Visitor& v) const { v.visit(*this); }
}

