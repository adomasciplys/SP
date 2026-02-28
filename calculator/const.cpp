#include "const.hpp"

namespace calculator
{
    const_t::const_t(double value) : value{value}
    {
    }

    double const_t::operator()(state_t&) const { return value; }

    void const_t::accept(Visitor& v) const { v.visit(*this); }
}

