#ifndef CALCULATOR_CONST_HPP
#define CALCULATOR_CONST_HPP

#include "term.hpp"
#include "visitor.hpp"

namespace calculator
{
    /** Struct representing a constant value expression */
    struct const_t : term_t
    {
        void accept(Visitor& v) const override;
        const_t(double value);
        const double value;
    };
}

#endif // CALCULATOR_CONST_HPP
