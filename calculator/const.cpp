#include "const.hpp"

namespace calculator
{
    const_t::const_t(double value) : value{value}
    {
    }
    void const_t::accept(Visitor& v) const { v.visit(*this); }
}
