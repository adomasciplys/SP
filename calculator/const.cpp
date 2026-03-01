#include "const.hpp"

namespace calculator
{
    const_t::const_t(double value) : _value{value}
    {
    }
    double const_t::value() const {return _value;}
    void const_t::accept(Visitor& v) const { v.visit(*this); }
}

