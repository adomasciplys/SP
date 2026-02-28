#ifndef CALCULATOR_OP_HPP
#define CALCULATOR_OP_HPP

#include <stdexcept>

namespace calculator
{
    /** Enum representing the operations we can do */
    enum op_t
    {
        ///< operator sign for assignment/unary/binary expression
        plus, ///< unary plus, like +v
        minus, ///< unary minus, like -v
        add, ///< binary addition, like v+v
        sub, ///< binary addition, like v-v
        assign, ///< assignment, like v <<= v
        mult, ///< binary multiplication, like v * v
        div ///< binary division, like v / v
    };

    /** Small helper to check for division by 0 */
    inline double safe_divide(const double a, const double b) {
        if (b == 0.0)
            throw std::logic_error{"division by zero"};
        return a / b;
    }
}

#endif // CALCULATOR_OP_HPP

